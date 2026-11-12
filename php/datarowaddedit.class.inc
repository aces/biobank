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
        return [];
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
        return new \LORIS\Http\Response\JSON\MethodNotAllowed(
            $this->allowedMethods()
        );
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
            'COLLECTION_TECHNICIAN',
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

        //validate temperature is in range 
        if (isset($values['TEMPERATURE']) && is_numeric($values['TEMPERATURE']) && ((float) $values['TEMPERATURE'] < -273.15 || (float) $values['TEMPERATURE'] > 100)) {
            $errors['TEMPERATURE'] = "must_be_between_-273.15_and_100. (inputed temperature: ".$values['TEMPERATURE'];
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