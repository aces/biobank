<?php declare(strict_types=1);
namespace LORIS\biobank;

use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;

/**
 * Post method handles add a data row.
 * This class contains functions providing add and edit functionality.
 *
 * @license http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 */
class DataRowAddEdit extends \LORIS\cbigr_api\Endpoint
{
    public $skipTemplate = true;

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
                'biobank_edit'
            ]
        );
    }


    protected function allowedMethods(): array
    {
        return [
            "POST",
            "PUT",
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
            case "POST":
                return $this->addSpecimenData($request);
            case "PUT":
                return $this->editSpecimenData($request);
            default:
                return (new \LORIS\Http\Response\JSON\MethodNotAllowed(
                    ["POST","PUT"]
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
    function addSpecimenData(ServerRequestInterface $request): ResponseInterface
    {
        // Commented because no FE adding functionality yet -- this endpoint
        // is currently only exercised indirectly via CSVImport::parseCSV(),
        // which calls DataRowAddEdit::validate() directly rather than
        // hitting this route. Left as a stub matching the genomics module's
        // pattern until a single-row add form exists for biobank specimens.

        // $values = json_decode((string)$request->getBody(), true);
        // $loris = $request->getAttribute('loris');
        // $DB = $loris->getDatabaseConnection();

        // $errors = $this->validate($values, $DB);
        // if (empty($errors)) {
        //     $DB->insert("biobank_container", ...);
        //     return new \LORIS\Http\Response\JSON\Created(
        //         ['message' => 'Created successfully']
        //     );
        // }

        // return new \LORIS\Http\Response\JSON\InternalServerError(
        //     "An unhandled exception prevented the data from being saved.
        //     Please contact your administrator."
        // );
    }

    /**
     * Handles the document editing process
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface
     */
    function editSpecimenData(ServerRequestInterface $request): ResponseInterface
    {
        // Commented because no FE editing functionality yet

        // $values = json_decode((string)$request->getBody(), true);
        // $loris = $request->getAttribute('loris');
        // $DB = $loris->getDatabaseConnection();

        // $barcode = $DB->pselectOne(
        //     "SELECT Barcode FROM biobank_container WHERE Barcode=:barcode",
        //     ["barcode"=>$values['BARCODE']]
        // );
        // if (empty($barcode)) {
        //     return  new \LORIS\Http\Response\JSON\BadRequest(
        //         "This specimen does not exist"
        //     );
        // }
    }

    /**
     * Validates the biobank data in a CSV row (or manually submitted form).
     *
     * NOTE: Some checks in this function are redundant with the database but are
     * listed to provide a more elegant error handling when parsing and importing
     * CSVs than waiting for an SQL failure
     *
     * NOTE: CSVImport::parseCSV() runs its own required-field check on
     * BARCODE, SPECIMEN_TYPE, PSCID, CONTAINER_TYPE, UNIT, QUANTITY before
     * ever calling this function, so those overlap by design (defense in
     * depth). PROJECTS is required here because parseCSV() currently reads
     * it unconditionally before any validation runs -- if that ordering
     * changes, keep this required check regardless, since every row now
     * needs a resolvable project.
     *
     * @param array     $values Row values, keyed by data-dictionary column
    * @param \Database $DB     Database
    * @param bool      $allowBlankPSCID Allow CSV imports to create a PSCID
    *                                   when the field is blank
     *
     * @return array
     */
    static function validate(
        array $values,
        \Database $DB,
        bool $allowBlankPSCID = true
    ) : array
    {
        $required = [
            'BARCODE',
            'SPECIMEN_TYPE',
            'CONTAINER_TYPE',
            'UNIT',
            'QUANTITY',
            'PROJECTS',
            'PROTOCOL',
            'CURRENT_SITE',
            'DRAW_SITE',
            'VISIT_LABEL',
            'COLLECTION_DATE',
            'SPECIMEN_STATUS',
        ];
        if (!$allowBlankPSCID) {
            $required[] = 'PSCID';
        }

        $errors = [];
        $creatingCandidate = null;
        // Check required fields
        foreach ($required as $field) {
            if (!isset($values[$field])) {
                $errors[$field] = "required";
            }
        }

        if (isset($values['COLLECTION_DATE']) && !preg_match('/^\d{4}-\d{2}-\d{2}$/', $values['COLLECTION_DATE'])) {
            $errors['COLLECTION_DATE'] = "must be in YYYY-MM-DD format";
        }
        // SEX is only required when PSCID doesn't already match an existing
        // candidate -- CSVImport::parseCSV() creates a new candidate in that
        // case and needs SEX to do so.
        if (isset($values['PSCID'] )) {
            $existingCandID = $DB->pselectOne(
                "SELECT CandID FROM candidate WHERE PSCID = :pscid",
                ["pscid" => $values['PSCID']]
            );
            if (empty($existingCandID)) {
                $creatingCandidate = true;
            }
        } else {
            $creatingCandidate = true;
        }

        if ($creatingCandidate) {
            if (!isset($values['DOB']) || $values['DOB'] === '') {
                $errors['DOB'] = "Required for new candidate";
            } elseif (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $values['DOB'])) {
                $errors['DOB'] = "DOB must be in YYYY-MM-DD format";
            }
            if (!isset($values['SEX']) || $values['SEX'] === "") {
                $errors['SEX'] = "Required for new candidate";
            }
        }



        // Enum / lookup-table membership checks. These confirm the value
        // exists in the referenced table but do not resolve IDs -- that
        // happens separately in CSVImport::parseCSV().
        $specimenTypeOptions = $DB->pselectCol(
            "SELECT Label FROM biobank_specimen_type",
            []
        );
        $containerTypeOptions = $DB->pselectCol(
            "SELECT Label FROM biobank_container_type",
            []
        );
        $unitOptions = $DB->pselectCol(
            "SELECT Label FROM biobank_unit",
            []
        );
        $siteOptions = $DB->pselectCol(
            "SELECT Name FROM psc",
            []
        );
        $examinerOptions = $DB->pselectCol(
            "SELECT full_name FROM examiners",
            []
        );
        $sexOptions = $DB->pselectCol(
            "SELECT Name FROM sex",
            []
        );
        // ASSUMPTION: projects are validated against LORIS core's Project
        // table (Name column) via \Project::singleton() in CSVImport.php.
        // Confirm the table/column name -- if project names actually live
        // elsewhere (e.g. a biobank-specific project table), update here.
        $projectOptions = $DB->pselectCol(
            "SELECT Name FROM Project",
            []
        );
        $visitLabelOptions = $DB->pselectCol(
            "SELECT VisitLabel FROM visit",
            []
        );
        $specimenStatusOptions = $DB->pselectCol(
            "SELECT Label FROM biobank_container_status",
            []
        );

        $validOptions = [
            "SPECIMEN_TYPE"  => $specimenTypeOptions,
            "CONTAINER_TYPE" => $containerTypeOptions,
            "UNIT"           => $unitOptions,
            "CURRENT_SITE"   => $siteOptions,
            "DRAW_SITE"      => $siteOptions,
            "COLLECTION_TECHNICIAN" => $examinerOptions,
            "SEX"            => $sexOptions,
            "PROJECTS"       => $projectOptions,
            "VISIT_LABEL"    => $visitLabelOptions,
            "SPECIMEN_STATUS"  => $specimenStatusOptions,
        ];

        foreach ($validOptions as $field => $options) {
            if (isset($values[$field]) && !in_array($values[$field], $options, true)) {
                $errors[$field] = "invalid_enum ".$values[$field];
            }
        }

        if (isset($values['PROTOCOL'], $values['SPECIMEN_TYPE'])) {
            $matchingProtocol = $DB->pselectOne(
                "SELECT bp.SpecimenProtocolID
                 FROM biobank_specimen_protocol bp
                 JOIN biobank_specimen_type st
                   ON st.SpecimenTypeID = bp.SpecimenTypeID
                 WHERE bp.Label = :protocol
                   AND st.Label = :specimenType",
                [
                    'protocol' => $values['PROTOCOL'],
                    'specimenType' => $values['SPECIMEN_TYPE'],
                ]
            );
            if (empty($matchingProtocol)) {
                $errors['PROTOCOL'] =
                    "invalid_for_specimen_type ".$values['PROTOCOL'];
            }
        }

        // DIAGNOSIS is semicolon-delimited (multiple diagnoses per
        // specimen), so each piece needs to resolve individually.
        if (isset($values['DIAGNOSIS'])) {
            $diagnosisOptions = $DB->pselectCol(
                "SELECT Name FROM diagnosis",
                []
            );
            $diagnoses = array_unique(array_map('trim', explode(';', $values['DIAGNOSIS'])));
            foreach ($diagnoses as $d) {
                if (!in_array($d, $diagnosisOptions, true)) {
                    $errors['DIAGNOSIS'] = "invalid_enum ".$d;
                    break;
                }
            }
        }

        // Numeric fields
        $numericFields = [
            'QUANTITY',
            'TEMPERATURE',
        ];
        foreach ($numericFields as $field) {
            if (isset($values[$field]) && !is_numeric($values[$field])) {
                $errors[$field] = "invalid_number ".$values[$field];
            }
        }
        if (isset($values['QUANTITY']) && is_numeric($values['QUANTITY']) && (float) $values['QUANTITY'] <= 0) {
            $errors['QUANTITY'] = "must_be_positive ".$values['QUANTITY'];
        }

        // Validate dates
        $dateFields = [
            'COLLECTION_DATE',
        ];

        foreach ($dateFields as $field) {
            if (isset($values[$field]) && (!preg_match('/^\d{4}-\d{2}-\d{2}$/', $values[$field]) || !(\DateTime::createFromFormat('Y-m-d', $values[$field])))) {
                $errors[$field] = "invalid_date ". $values[$field];
            }
        }

        return $errors;
    }

    /**
     * Check fields redundant with LORIS to make sure info matches. If not, return warnings
     *
     * NOTE: unlike the genomics module (where CandID is already known when
     * this runs), CSVImport::parseCSV() may not yet have resolved or
     * created a candidate for this row -- if a candidate exists, checks
     * below run against their current DB values. If a candidate is being
     * newly created, there is nothing yet to conflict against, so
     * conflicts are only meaningful for pre-existing PSCIDs.
     *
     * @param array     $values Row values, keyed by data-dictionary column
     * @param \Database $DB     Database
     *
     * @return array
     */

    static function detectConflicts(array $values, \Database $DB) : array
    {
        $conflicts = [];

        if (!isset($values['PSCID'])) {
            return $conflicts;
        }

        $candID = $DB->pselectOne(
            "SELECT CandID FROM candidate WHERE PSCID = :pscid",
            ["pscid" => $values['PSCID']]
        );
        if (empty($candID)) {
            // New candidate -- nothing in the DB yet to conflict with.
            return $conflicts;
        }

        if (isset($values['SEX'])) {
            $sex = $DB->pselectOne(
                "SELECT Sex FROM candidate WHERE CandID = :cid",
                ["cid" => $candID]
            );
            if ($sex !== null && $sex !== $values['SEX']) {
                $conflicts['SEX'] = "DB: $sex - CSV: ".$values['SEX'];
            }
        }

        return $conflicts;
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