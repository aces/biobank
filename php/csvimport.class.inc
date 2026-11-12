<?php declare(strict_types=1);
namespace LORIS\biobank;

use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;
use \LORIS\biobank\DataRowAddEdit;
use LORIS\StudyEntities\Candidate\CandID;
use \LORIS\StudyEntities\Candidate\Sex;

/**
 * Post method handles add.
 * This class contains functions providing add and edit functionality.
 *
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 */
class CSVImport extends \LORIS\cbigr_api\Endpoint
{
    public $skipTemplate = true;

    // TODO: confirm the correct Project name new candidates should register
    // under for biobank-only intake. Left as a placeholder distinct from the
    // genomics importer's "Neuro Bioinformatics Core".
    //const BIOBANK_PROJECT_NAME = 'TODO_SET_PROJECT_NAME';

    /**
     * Same as the permissions for the main menu filter page.
     *
     * @param \User $user The user whose access is being checked
     *
     * @return bool true only if the user has access to this page.
     */
    function _hasAccess(\User $user) : bool
    {
        return $user->hasAnyPermission(
            [
                'biobank_specimen_create'
            ]
        );
    }


    protected function allowedMethods(): array
    {
        return [
            "GET",
            "POST",
        ];
    }

    /**
     * Handle how to operate all the files.
     * POST method uploads a file.
     * GET method gets a file.
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(ServerRequestInterface $request): ResponseInterface
    {
        switch ($request->getMethod()) {
            case "GET":
                return $this->downloadErrorLog($request);
            case "POST":
                return $this->importCSV($request);
            default:
                return (new \LORIS\Http\Response\JSON\MethodNotAllowed(
                    $this->allowedMethods()
                ));
        }
    }

    /**
     * Handles the document editing process
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface
     */
    function importCSV(ServerRequestInterface $request): ResponseInterface
    {
        $loris = $request->getAttribute('loris');
        $DB = $loris->getDatabaseConnection();
        $fileDate = date('Ymd_His');

        $targetdir = "/data/biobank/imports/";
        $logdir = "/data/biobank/logs/";
        // Ensure the server is properly configured.
        if (!is_writable($targetdir)) {
            return new \LORIS\Http\Response\JSON\Forbidden(
                "User does not have write permission on filesystem"
            );
        }

        $uploaded_file = $request->getUploadedFiles()['csvFile'];
        $spl           = new \SplFileInfo($targetdir);
        $filename      = $uploaded_file->getClientFilename(); //blabla.csv
        $fullPath      = $spl->getPathname().'/'.$filename; // /path/to/imports/blabla.csv
        $fullPathDatedNoExt = $spl->getPathname().'/'.basename($filename,".csv")."_".$fileDate;
        $fullPathDated = $fullPathDatedNoExt.".csv";
        $logPathDated = str_replace("/imports/","/logs/",$fullPathDated);

        $uploader = (new \LORIS\FilesUploadHandler($spl))
            ->withPermittedMIMETypes(
                'text/csv',
                'application/vnd.ms-excel',
            );

        $response = $uploader->handle($request);

        if (!in_array($response->getStatusCode(), [200, 201], true)) {
            return $response;
        }
        // move file after upload because FileUploadHandler
        // can't rename file to append a date
        rename($fullPath,$fullPathDated);

        $output = $this->parseCSV(
            $fullPathDated,
            $logPathDated,
            $DB,
            $request->getAttribute('user')
        );

        $body = [
            'message' => count($output['new'])." New Entries. ".
                count($output['errors'])." Errors. ".
                count($output['ignored'])." Ignored.",
        ];

        if (count($output['errors']) > 0 || count($output['ignored']) > 0) {
            $body['log'] = basename($logPathDated);
        }

        return new \LORIS\Http\Response\JSON\Created($body);
    }

    /**
     * function dedicated to the parse logic
     *
     * This function can be run using the tools/CSVImportRunner.php
     *
     * Expected CSV headers (per data dictionary):
     *   BARCODE, SPECIMEN_TYPE, PSCID, DOB, DIAGNOSIS, CURRENT_SITE, DRAW_SITE,
     *   COLLECTION_DATE, CONTAINER_TYPE, UNIT, QUANTITY, TEMPERATURE,
     *   PROTOCOL, COLLECTION_TECHNICIAN, SEX (required only when PSCID is new)
     *
     * Row processing order:
     * 1. Check required fields (BARCODE, SPECIMEN_TYPE, PSCID, CONTAINER_TYPE,
     *    UNIT, QUANTITY, PROTOCOL, SPECIMEN_STATUS) are present => if not ERROR & SKIP
     * 2. Check if BARCODE is a duplicate within this file, or already exists
     *    in the DB (prior import / manual entry) => IGNORE & SKIP
     * 3. Run DataRowAddEdit::validate() on the row => if problem ERROR & SKIP
     * 4. Resolve lookup fields (SPECIMEN_TYPE, UNIT, CONTAINER_TYPE, sites,
     *    COLLECTION_TECHNICIAN, PROTOCOL, DIAGNOSIS) against the DB => if any fail to
     *    resolve ERROR & SKIP
     * 5. Check if PSCID matches an existing candidate => if not, CREATE one
     *    (requires SEX in the row)
     * 6. Get or create the candidate's session
     * 7. Determine the next SampleNumber for that session
     * 8. Insert biobank_container
     * 9. Insert biobank_specimen + biobank_specimen_collection
     * 10. Insert any new candidate_diagnosis_rel links (skip existing ones)
     *
     * @param string    $fullPathDated Path to the CSV that needs to be parsed
     * @param string    $logPathDated  Path to the log files
     * @param \Database $DB            Database
     *
     * @return array $arr Array containing import results
     */
    public function parseCSV(
        string $fullPathDated,
        string $logPathDated,
        \Database $DB,
        \User $user
    ) : array
    {
        $file = fopen($fullPathDated,"r");
        $headers = fgetcsv($file);
        $logActive = fopen($logPathDated.".active.log","w");

        $errors = [];
        $ignored = [];
        $new = [];
        $failed = false;

        //TODO: do we want/need a fallback center for candidate creation? Genomics importer uses this but that may not be desired behavior.
        // $biobankProjectID = \Project::singleton(self::BIOBANK_PROJECT_NAME)->getId();

        /**
         * ---- Lookup caches, loaded once before the loop ----
         */
        // Candidate info is needed to source a center/project when creating
        // a session for a candidate resolved by external ID.
        $candidateRows = $DB->pselect(
            "SELECT CandID, PSCID, RegistrationCenterID, RegistrationProjectID
             FROM candidate WHERE PSCID <> 'scanner'",
            []
        );
        $candidateById = [];
        foreach ($candidateRows as $c) {
            $candidateById[$c['CandID']] = $c;
        }

        $NBCextID = $DB->pselectOne(
            "SELECT ProjectExternalID FROM Project_external WHERE Name = :name",
            ['name' => 'NBC']
        );
        $externalIDRows = $DB->pselectColWithIndexKey(
            "SELECT cper.ExtStudyID, MAX(cper.CandID) AS CandID
             FROM candidate_project_extid_rel cper
             JOIN Project_external pe
               ON pe.ProjectExternalID = cper.ProjectExternalID
             WHERE pe.Name = :name
             GROUP BY cper.ExtStudyID",
            ['name' => 'NBC'],
            'ExtStudyID'
        );
        $externalIDList = [];
        foreach ($externalIDRows as $externalID => $candID) {
            $externalIDList[$this->normalizeCsvKey((string) $externalID)] = $candID;
        }

        $siteList = $DB->pselectColWithIndexKey(
            "SELECT CenterID, Name FROM psc",
            [],
            "Name"
        );

        $specimenTypeList = $DB->pselectColWithIndexKey(
            "SELECT SpecimenTypeID, Label FROM biobank_specimen_type",
            [],
            "Label"
        );

        $unitList = $DB->pselectColWithIndexKey(
            "SELECT UnitID, Label FROM biobank_unit",
            [],
            "Label"
        );

        $containerTypeList = $DB->pselectColWithIndexKey(
            "SELECT ContainerTypeID, Label FROM biobank_container_type",
            [],
            "Label"
        );
        $containerStatusList = $DB->pselectColWithIndexKey(
            "SELECT ContainerStatusID, Label FROM biobank_container_status",
            [],
            "Label"
        );

        // $examinerList = $DB->pselectColWithIndexKey(
        //     "SELECT examinerID, full_name FROM examiners",
        //     [],
        //     "full_name"
        // );
        $exRows = $DB->pselect(
        "SELECT examinerID, full_name FROM examiners",
        []
        );
        $examinerList = [];
        foreach ($exRows as $exRow) {
            $examinerList[$exRow['full_name']] = $exRow['examinerID'];
        }

        $diagnosisList = $DB->pselectColWithIndexKey(
            "SELECT DiagnosisID, Name FROM diagnosis",
            [],
            "Name"
        );

        $projectList = $DB->pselectCol("SELECT Name FROM Project", []);
        $sexList = $DB->pselectCol("SELECT Name FROM sex", []);
        $visitLabels = $DB->pselectCol(
            "SELECT VisitLabel FROM visit",
            []
        );

        // Protocols are specific to a specimen type, so key on both.
        $protocolRows = $DB->pselect(
            "SELECT SpecimenProtocolID, Label, SpecimenTypeID FROM biobank_specimen_protocol",
            []
        );
        $protocolList = [];
        foreach ($protocolRows as $p) {
            $protocolList[$p['Label'].'_'.$p['SpecimenTypeID']] = $p['SpecimenProtocolID'];
        }
        $protocolLabelList = array_column($protocolRows, 'Label');

        // Existing barcodes -- used to catch duplicates against prior imports
        // or manually-registered specimens.
        $existingBarcodes = $DB->pselectCol(
            "SELECT Barcode FROM biobank_container",
            []
        );
        $existingBarcodeKeys = array_fill_keys(
            array_map([$this, 'normalizeCsvKey'], $existingBarcodes),
            true
        );

        // Existing sessions keyed by candidate and the CSV-provided visit.
        $sessionRows = $DB->pselect(
            "SELECT CandID, ID, Visit_label FROM session",
            []
        );
        $sessionList = [];
        foreach ($sessionRows as $session) {
            $sessionKey = $session['CandID'].'|'.
                $this->normalizeCsvKey($session['Visit_label']);
            $sessionList[$sessionKey] = $session['ID'];
        }

        // Running per-candidate SampleNumber counters, seeded from current max.
        $sampleNumberCounts = $DB->pselectColWithIndexKey(
            "SELECT s.CandID, MAX(bs.SampleNumber) AS MaxNum
             FROM biobank_specimen bs
             JOIN session s ON bs.SessionID = s.ID
             GROUP BY s.CandID",
            [],
            "CandID"
        );

        $rowNumber = 1;
        while (($line=fgetcsv($file)) !== FALSE) {
            $rowNumber++;
            // Override default execution time to 10 seconds per iteration
            // (still catches infinite loops)
            set_time_limit(10);
            //TODO MAKE SUPPORT FOR MULTIPLE PROJECTS
            $failed = false;
            $row = array_filter(
                array_combine(
                    array_map([$this, 'normalizeCsvHeader'], $headers),
                    array_map([$this, 'normalizeCsvValue'], $line)
                ),
                function($cell) {
                    return $cell !== '' && strtolower($cell) !== "na";
                }
            );

            if (!isset($row['BARCODE']) || trim((string) $row['BARCODE']) === '') {
                $ignoredRow = 'ROW_'.$rowNumber;
                $ignored[] = $ignoredRow;
                fwrite($logActive, $ignoredRow." --> ignored --> BARCODE IS BLANK\n");
                error_log('[CSVImport] Ignored CSV row '.$rowNumber.' without a barcode');
                continue;
            }

            foreach (
                [
                    'PROJECTS'        => $projectList,
                    'SPECIMEN_TYPE'   => $specimenTypeList,
                    'UNIT'            => $unitList,
                    'CONTAINER_TYPE'  => $containerTypeList,
                    'CURRENT_SITE'    => $siteList,
                    'DRAW_SITE'       => $siteList,
                    'COLLECTION_TECHNICIAN' => $examinerList,
                    'PROTOCOL'        => $protocolLabelList,
                    'SEX'             => $sexList,
                    'VISIT_LABEL'     => $visitLabels,
                ] as $field => $options
            ) {
                if (isset($row[$field]) && $options) {
                    $row[$field] = $this->canonicalizeCsvLookup(
                        $row[$field],
                        $options
                    );
                }
            }
            if (isset($row['DIAGNOSIS'])) {
                $row['DIAGNOSIS'] = implode(
                    ';',
                    array_map(
                        function ($diagnosis) use ($diagnosisList) {
                            return $this->canonicalizeCsvLookup(
                                $diagnosis,
                                $diagnosisList
                            );
                        },
                        explode(';', $row['DIAGNOSIS'])
                    )
                );
            }
            $ukey = $row['BARCODE'] ?? ('MISSING_BARCODE_'.uniqid());
            fwrite($logActive, $ukey."\n");
            error_log(
                '[CSVImport] Processing row '.$ukey.' with fields: '.
                implode(',', array_keys($row))
            );

            // 1. Required fields present
            foreach (['BARCODE','PROJECTS','SPECIMEN_TYPE','CONTAINER_TYPE','UNIT','QUANTITY','PROTOCOL', 'CURRENT_SITE', 'DRAW_SITE', 'VISIT_LABEL', 'SPECIMEN_STATUS', 'COLLECTION_TECHNICIAN'] as $required) {
                if (!isset($row[$required])) {
                    $errors[$ukey]['missing_field'][] = $required;
                    $failed = true;
                }
            }
            if ($failed) {
                fwrite($logActive, $ukey." --> missing_field --> ".implode(',', $errors[$ukey]['missing_field'])."\n");
                error_log(
                    '[CSVImport] Rejected row '.$ukey.' missing fields: '.
                    implode(',', $errors[$ukey]['missing_field'])
                );
                continue;
            }

            $projectName = trim($row['PROJECTS']);
            if (!in_array($projectName, $projectList, true)) {
                $errors[$ukey]['PROJECTS'] = "Unknown project '{$projectName}'";
                fwrite($logActive, $ukey." --> PROJECTS --> not found: ".$projectName."\n");
                error_log(
                    '[CSVImport] Rejected row '.$ukey.' with unknown project '.$projectName
                );
                continue;
            }
            try {
                $project = \Project::singleton($projectName);
                $biobankProjectID = (int) (string) $project->getId();
            } catch (\Throwable $e) {
                $errors[$ukey]['PROJECTS'] = "Could not resolve project '{$projectName}'";
                fwrite($logActive, $ukey." --> PROJECTS --> ".$errors[$ukey]['PROJECTS']."\n");
                error_log(
                    '[CSVImport] Could not resolve project for row '.$ukey.': '.$e->getMessage()
                );
                continue;
            }

            // 2. Duplicate within this same file
            if (in_array($ukey, array_merge($ignored, $new), true) || array_key_exists($ukey, $errors)) {
                $errors[$ukey]['duplication'] = "DUPLICATE IN SAME FILE";
                fwrite($logActive, $ukey." --> duplication --> DUPLICATE IN SAME FILE\n");
                error_log('[CSVImport] Rejected duplicate row '.$ukey);
                continue;
            }

            // Already-registered barcode -- ignore rather than error, same as
            // the genomics importer treated already-imported rows.
            if (isset($existingBarcodeKeys[$this->normalizeCsvKey($row['BARCODE'])])) {
                $ignored[] = $ukey;
                fwrite($logActive, $ukey." --> ignored --> BARCODE ALREADY EXISTS\n");
                error_log('[CSVImport] Ignored existing barcode '.$ukey);
                continue;
            }

            // 3. VALIDATE
            // TODO: confirm DataRowAddEdit::validate()'s required-field list
            // (written for the manual add/edit form) matches these columns. 
            $dataErrors = DataRowAddEdit::validate($row, $DB, true);
            if (!empty($dataErrors)) {
                $errors[$ukey]['data'] = $dataErrors;
                fwrite($logActive, $ukey." --> data --> ".print_r($dataErrors,true)."\n");
                error_log(
                    '[CSVImport] Rejected row '.$ukey.' validation errors: '.
                    json_encode($dataErrors)
                );
                continue;
            }

            if (!in_array($row['VISIT_LABEL'], $visitLabels, true)) {
                $errors[$ukey]['VISIT_LABEL'] =
                    "Unknown visit label '{$row['VISIT_LABEL']}'";
                fwrite($logActive, $ukey." --> VISIT_LABEL --> not found\n");
                error_log(
                    '[CSVImport] Rejected row '.$ukey.
                    ' with unknown visit label '.$row['VISIT_LABEL']
                );
                continue;
            }
            // 4. Resolve lookup fields
            $specimenTypeID = $specimenTypeList[$row['SPECIMEN_TYPE']] ?? null;
            if ($specimenTypeID === null) {
                $errors[$ukey]['SPECIMEN_TYPE'] = "Unknown specimen type '{$row['SPECIMEN_TYPE']}'";
                fwrite($logActive, $ukey." --> SPECIMEN_TYPE --> not found\n");
                continue;
            }
            $specimen_statusID = $containerStatusList[$row['SPECIMEN_STATUS']] ?? null;
            if ($specimen_statusID === null) {
                $errors[$ukey]['SPECIMEN_STATUS'] = "Unknown specimen status '{$row['SPECIMEN_STATUS']}'";
                fwrite($logActive, $ukey." --> SPECIMEN_STATUS --> not found\n");
                continue;
            }

            $unitID = $unitList[$row['UNIT']] ?? null;
            if ($unitID === null) {
                $errors[$ukey]['UNIT'] = "Unknown unit '{$row['UNIT']}'";
                fwrite($logActive, $ukey." --> UNIT --> not found\n");
                continue;
            }

            $containerTypeID = $containerTypeList[$row['CONTAINER_TYPE']] ?? null;
            if ($containerTypeID === null) {
                $errors[$ukey]['CONTAINER_TYPE'] = "Unknown container type '{$row['CONTAINER_TYPE']}'";
                fwrite($logActive, $ukey." --> CONTAINER_TYPE --> not found\n");
                continue;
            }

            // $currentSiteID = $siteList[$row['CURRENT_SITE'] ?? ''] ?? null;
            // $drawSiteID = $siteList[$row['DRAW_SITE'] ?? ''] ?? null;
            $currentSiteID = $siteList[$row['CURRENT_SITE']] ?? null;
            if ($currentSiteID === null) {
                $errors[$ukey]['CURRENT_SITE'] = "Unknown site '{$row['CURRENT_SITE']}'";
                fwrite($logActive, $ukey." --> CURRENT_SITE --> not found\n");
                continue;
            }

            $drawSiteID = $siteList[$row['DRAW_SITE']] ?? null;
            if ($drawSiteID === null) {
                $errors[$ukey]['DRAW_SITE'] = "Unknown site '{$row['DRAW_SITE']}'";
                fwrite($logActive, $ukey." --> DRAW_SITE --> not found\n");
                continue;
            }

            $examinerID = null;
            if (isset($row['COLLECTION_TECHNICIAN'])) {
                error_log("COLLECTION_TECHNICIAN: ".$row['COLLECTION_TECHNICIAN']);
                $examinerID = $examinerList[$row['COLLECTION_TECHNICIAN']];
                $examinerID = $examinerList[$row['COLLECTION_TECHNICIAN']] ?? null;
                if ($examinerID === null) {
                    $errors[$ukey]['COLLECTION_TECHNICIAN'] = "Unknown examiner '{$row['COLLECTION_TECHNICIAN']}'";
                    fwrite($logActive, $ukey." --> COLLECTION_TECHNICIAN --> not found\n");
                    continue;
                }
            }


            // PROTOCOL is required (checked in step 1 above) since
            // biobank_specimen_collection.SpecimenProtocolID is NOT NULL.
            //TODO: confirm user will specify protocol!
            $protocolID = $protocolList[$row['PROTOCOL'].'_'.$specimenTypeID] ?? null;
            if ($protocolID === null) {
                $errors[$ukey]['PROTOCOL'] = "No protocol '{$row['PROTOCOL']}' defined for specimen type '{$row['SPECIMEN_TYPE']}'";
                fwrite($logActive, $ukey." --> PROTOCOL --> not found\n");
                continue;
            }

            $diagnoses = isset($row['DIAGNOSIS'])
                ? array_unique(
                    array_map(
                        function ($diagnosis) use ($diagnosisList) {
                            return $this->canonicalizeCsvLookup(
                                $diagnosis,
                                $diagnosisList
                            );
                        },
                        explode(';', $row['DIAGNOSIS'])
                    )
                )
                : [];
            $diagnosisIDs = [];
            foreach ($diagnoses as $d) {
                if (!isset($diagnosisList[$d])) {
                    $errors[$ukey]['DIAGNOSIS'] = "Unknown diagnosis '$d'";
                    $failed = true;
                    break;
                }
                $diagnosisIDs[] = $diagnosisList[$d];
            }
            if ($failed) {
                fwrite($logActive, $ukey." --> DIAGNOSIS --> not found\n");
                continue;
            }
            /**
             * 8/9/10. Insert data in respective tables
             */
            $newCandidateCacheEntry = null;
            $newExternalIDCacheEntry = null;
            $newSessionCacheEntry = null;
            $DB->beginTransaction();
            try {
                // 5. Resolve or create candidate using the NBC external ID.
                $externalID = trim((string) ($row['PSCID'] ?? ''));
                $externalIDKey = $this->normalizeCsvKey($externalID);
                $DOB = $row['DOB'] ?? null;
                $candidateID = $externalIDKey === ''
                    ? null
                    : ($externalIDList[$externalIDKey] ?? null);
                $candidateInfo = $candidateID === null
                    ? null
                    : ($candidateById[$candidateID] ?? null);
                if ($candidateID !== null && $candidateInfo === null) {
                    $errors[$ukey]['EXTID'] =
                        "NBC external ID '{$externalID}' references an unavailable candidate";
                    fwrite($logActive, $ukey." --> EXTID --> candidate not found\n");
                    $DB->rollBack();
                    continue;
                }
                if ($candidateInfo === null) {
                    if (!isset($row['SEX'])) {
                        $errors[$ukey]['SEX'] = "SEX is required to create new candidate for PSCID '{$externalID}'";
                        fwrite($logActive, $ukey." --> SEX --> required for new candidate\n");
                        $DB->rollBack();
                        continue;
                    }

                    $newCandCenter = $currentSiteID !== null
                        ? new \CenterID((string) $currentSiteID)
                        : $fallbackCenterID;

                    try {
                        error_log(
                            '[CSVImport] CREATE candidate for row '.$ukey.': '.
                            json_encode(
                                [
                                    'ExternalID' => $externalID,
                                    'CenterID' => (string) $newCandCenter,
                                    'ProjectID' => $biobankProjectID,
                                    'Sex' => $row['SEX'],
                                ],
                                JSON_UNESCAPED_SLASHES
                            )
                        );
                    //TODO FIX CANDIDATE CREATION. replace Default DOB logic. Sex logic?
                        $cid = (string) \Candidate::createNew(
                            $newCandCenter,
                            $DOB,
                            null,
                            new Sex($row['SEX']),
                            //$pscid,
                            null,
                            new \ProjectID((string) $biobankProjectID),
                        );
                    } catch (\Exception $e) {
                        $errors[$ukey]['CANDIDATE'] = $e->getMessage();
                        fwrite($logActive, $ukey." --> CANDIDATE --> ".$e->getMessage()."\n");
                        $DB->rollBack();
                        continue;
                    }
                    fwrite($logActive, $ukey." --> New Candidate --> ".$cid."\n");

                    $externalIDData = [
                        'CandID' => $cid,
                        'ProjectExternalID' => $NBCextID,
                        'ExtStudyID' => $externalID,
                    ];
                    $this->logDatabaseInsert(
                        'candidate_project_extid_rel',
                        $externalIDData,
                        $ukey
                    );
                    $DB->unsafeinsert(
                        'candidate_project_extid_rel',
                        $externalIDData
                    );

                    $candidateInfo = [
                        'CandID'                => $cid,
                        'PSCID'                 => null,
                        'RegistrationCenterID'  => (int) (string) $newCandCenter,
                        'RegistrationProjectID' => $biobankProjectID,
                    ];
                    $newCandidateCacheEntry = [
                        'id' => $cid,
                        'info' => $candidateInfo,
                    ];
                    $newExternalIDCacheEntry = [
                        'key' => $externalIDKey,
                        'id' => $cid,
                    ];
                }
                $cid = $candidateInfo['CandID'];

                // 6. Resolve or create the session for the CSV-provided visit
                $sessionKey = $cid.'|'.$this->normalizeCsvKey($row['VISIT_LABEL']);
                $sessionID = $sessionList[$sessionKey] ?? null;
                if ($sessionID === null) {
                    try {
                        $sessionID = $this->createBiospecimenSession(
                            $DB,
                            (int) $cid,
                            (int) $candidateInfo['RegistrationCenterID'],
                            $biobankProjectID,
                            $row['VISIT_LABEL'],
                            $row['COLLECTION_DATE'] ?? date('Y-m-d'),
                            $user
                        );
                    } catch (\DatabaseException | \PDOException $e) {
                        $errors[$ukey]['SESSION'] = $e->getMessage();
                        fwrite($logActive, $ukey." --> SESSION --> ".$e->getMessage()."\n");
                        $DB->rollBack();
                        continue;
                    }
                    $newSessionCacheEntry = [
                        'key' => $sessionKey,
                        'id' => $sessionID,
                    ];
                    fwrite($logActive, $ukey." --> New Session --> ".$sessionID."\n");
                }

                // 7. Next SampleNumber for this session
                $sampleNumberCounts[$cid] = ($sampleNumberCounts[$cid] ?? 0) + 1;
                $sampleNumber = $sampleNumberCounts[$cid];
                $containerData = [
                    'Barcode'           => $row['BARCODE'],
                    'ContainerTypeID'   => $containerTypeID,
                    'Temperature'       => $row['TEMPERATURE'] ?? 20.00,
                    'ContainerStatusID' => $specimen_statusID,
                    'CenterID'          => $currentSiteID,
                ];
                $this->logDatabaseInsert('biobank_container', $containerData, $ukey);
                $DB->unsafeinsert('biobank_container', $containerData);
                $containerID = $DB->getLastInsertId();

                $specimenData = [
                    'ContainerID'    => $containerID,
                    'SpecimenTypeID' => $specimenTypeID,
                    'Quantity'       => $row['QUANTITY'],
                    'UnitID'         => $unitID,
                    'SessionID'      => $sessionID,
                    'SampleNumber'   => $sampleNumber,
                ];
                $this->logDatabaseInsert('biobank_specimen', $specimenData, $ukey);
                $DB->unsafeinsert('biobank_specimen', $specimenData);
                $specimenID = $DB->getLastInsertId();

                $projectRelData = [
                    'SpecimenID' => $specimenID,
                    'ProjectID'  => $biobankProjectID,
                ];
                $this->logDatabaseInsert('biobank_specimen_project_rel', $projectRelData, $ukey);
                $DB->unsafeinsert('biobank_specimen_project_rel', $projectRelData);

                $collectionData = [
                    'SpecimenID'         => $specimenID,
                    'SpecimenProtocolID' => $protocolID,
                    'Quantity'           => $row['QUANTITY'],
                    'UnitID'             => $unitID,
                    'CenterID'           => $drawSiteID ?? $currentSiteID,
                    'ExaminerID'         => $examinerID,
                    'Date'               => $row['COLLECTION_DATE'] ?? date('Y-m-d'),
                    // ASSUMPTION: no COLLECTION_TIME in the dictionary.
                    'Time'               => $row['COLLECTION_TIME'] ?? '00:00:00',
                ];
                $this->logDatabaseInsert(
                    'biobank_specimen_collection',
                    $collectionData,
                    $ukey
                );
                $DB->unsafeinsert('biobank_specimen_collection', $collectionData);

                foreach ($diagnosisIDs as $did) {
                    // Insert only the first diagnosis for a candidate.
                    //TODO: Log entry not working for rows where diagnosis!=existing diagnosis (logic is fine)
                    $existingDiagnosis = $DB->pselectOne(
                        "SELECT d.Name
                         FROM candidate_diagnosis_rel cdr
                         JOIN diagnosis d ON d.DiagnosisID = cdr.DiagnosisID
                         WHERE cdr.CandID = :cid",
                        ['cid' => $cid]
                    );
                    $diagnosisAlreadyRegistered = $DB->pselectOne(
                        "SELECT COUNT(*) FROM candidate_diagnosis_rel
                         WHERE CandID = :cid AND DiagnosisID = :did",
                        ['cid' => $cid, 'did' => $did]
                    );
                    if (!$existingDiagnosis) {
                        $diagnosisData = ['CandID' => $cid, 'DiagnosisID' => $did];
                        $this->logDatabaseInsert(
                            'candidate_diagnosis_rel',
                            $diagnosisData,
                            $ukey
                        );
                        $DB->unsafeinsert('candidate_diagnosis_rel', $diagnosisData);
                    } else if (!$diagnosisAlreadyRegistered && $existingDiagnosis) {
                        $diagnosisName = array_search(
                            $did,
                            $diagnosisList
                        );
                        if ($diagnosisName === false) {
                            $diagnosisName = (string) $did;
                        }
                        fwrite(
                            $logActive,
                            $ukey.' --> Diagnosis '.$diagnosisName.
                            ' not inserted, candidate has existing diagnosis '.$
                            $existingDiagnosis."\n"
                        );
                    }
                }

                $existingBarcodes[] = $row['BARCODE'];
                $existingBarcodeKeys[$this->normalizeCsvKey($row['BARCODE'])] = true;
                $new[] = $ukey;
            } catch (\DatabaseException | \PDOException $e) {
                $DB->rollBack();
                // Roll the SampleNumber counter back too, since this row's
                // number was never actually consumed.
                $sampleNumberCounts[$cid]--;
                $errors[$ukey]['SQL'] = $e->getMessage();
                fwrite($logActive, $ukey." --> SQL --> ".$e->getMessage()."\n");
                error_log(
                    '[CSVImport] Database insert failed for row '.$ukey.': '.
                    $e->getMessage()
                );
                continue;
            }
            $DB->commit();
            if ($newCandidateCacheEntry !== null) {
                $candidateById[$newCandidateCacheEntry['id']] =
                    $newCandidateCacheEntry['info'];
            }
            if ($newExternalIDCacheEntry !== null) {
                $externalIDList[$newExternalIDCacheEntry['key']] =
                    $newExternalIDCacheEntry['id'];
            }
            if ($newSessionCacheEntry !== null) {
                $sessionList[$newSessionCacheEntry['key']] =
                    $newSessionCacheEntry['id'];
            }
            error_log('[CSVImport] Database inserts committed for row '.$ukey);
        }

        fclose($file);

        $log = fopen($logPathDated.".log","w");
        $logPretty = fopen($logPathDated.".pretty.log","w");
        $arr = [
                "errors"  => $errors,
                "ignored" => $ignored,
                "new"     => $new,
            ];
        fwrite($log, json_encode($arr));
        fwrite($logPretty, print_r($arr,true));
        fclose($log);
        fclose($logPretty);
        fclose($logActive);

        return $arr;
    }

    /**
     * Log a database insert attempt for a CSV row.
     *
     * @param string               $table Table receiving the insert
     * @param array<string, mixed> $data  Data being inserted
     * @param string               $ukey  CSV row identifier
     *
     * @return void
     */
    private function logDatabaseInsert(string $table, array $data, string $ukey): void
    {
        error_log(
            '[CSVImport] INSERT '.$table.' for row '.$ukey.': '.
            json_encode($data, JSON_UNESCAPED_SLASHES)
        );
    }

    /**
     * Normalize a CSV header name.
     *
     * @param string $value Header value
     *
     * @return string
     */
    private function normalizeCsvHeader(string $value): string
    {
        return strtoupper(trim($value));
    }

    /**
     * Normalize a CSV cell without changing its capitalization.
     *
     * @param mixed $value Cell value
     *
     * @return string
     */
    private function normalizeCsvValue($value): string
    {
        return trim((string) $value);
    }

    /**
     * Create a case-insensitive comparison key.
     *
     * @param string $value Value to normalize
     *
     * @return string
     */
    private function normalizeCsvKey(string $value): string
    {
        return strtolower(trim($value));
    }

    /**
     * Return the database's canonical spelling for a lookup value.
     *
     * @param string               $value   CSV value
     * @param array<int|string,mixed> $options Lookup values or label-keyed map
     *
     * @return string Original value when no match exists
     */
    private function canonicalizeCsvLookup(string $value, array $options): string
    {
        $key = $this->normalizeCsvKey($value);
        foreach ($options as $optionKey => $optionValue) {
            $candidate = is_string($optionKey) ? $optionKey : $optionValue;
            if ($this->normalizeCsvKey((string) $candidate) === $key) {
                return (string) $candidate;
            }
        }
        return trim($value);
    }

    /**
    * Creates a session for a candidate and visit label when one does not
    * already exist.
     *
     * ASSUMPTION: uses the candidate's own registration center/project as
     * the session's center/project, and leaves other session fields at
     * their table defaults (Active=Y, Current_stage='Not Started', etc).
     * Confirm this is the right source for CenterID/ProjectID before
     * relying on it -- it may need to come from the CSV row (CURRENT_SITE)
     * instead for candidates registered elsewhere.
     *
     * @param \Database $DB        Database
     * @param int       $candID    Candidate ID
     * @param int       $centerID  Center to attach the session to
     * @param int       $projectID Project to attach the session to
    * @param string    $visitLabel Visit label from the CSV
     *
     * @return int the new session's ID
     */
    private function createBiospecimenSession(
        \Database $DB,
        int $candID,
        int $centerID,
        int $projectID,
        string $visitLabel,
        string $visitDate,
        \User $user
    ) : int {
        $sessionData = [
            'CandID'       => $candID,
            'CenterID'     => $centerID,
            'ProjectID'    => $projectID,
            'Visit_label'  => $visitLabel,
            'UserID'       => $user->getId(),
            'Active'       => 'Y',
            'Submitted'    => 'N',
            'Current_stage' => 'Not Started',
            'Date_visit' => $visitDate,
        ];
        $this->logDatabaseInsert('session', $sessionData, 'CandID='.$candID);
        $DB->unsafeinsert('session', $sessionData);
        return (int) $DB->getLastInsertId();
    }

    /**
     * Serve log associated to CSV import attempt
     *
     * @param ServerRequestInterface $request
     * @return ResponseInterface
     */
    function downloadErrorLog(ServerRequestInterface $request): ResponseInterface
    {
        $filename = urldecode(basename($request->getUri()->getPath()));

        $targetdir = "/data/biobank/logs/";
        $spl       = new \SplFileInfo($targetdir);

        $downloader = new \LORIS\FilesDownloadHandler($spl);

        return $downloader->handle(
            $request->withAttribute('filename', $filename.".pretty.log")
        );
    }


    /**
     * This function can be overridden in a module's page to load the necessary
     * resources to check the permissions of a user.
     *
     * @param \User                   $user    The user to load the resources for
     * @param ServerRequestInterface $request The PSR15 Request being handled
     *
     * @return void
     */
    public function loadResources(
        \User $user, ServerRequestInterface $request
    ) : void {
    }

}
