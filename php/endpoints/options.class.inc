<?php declare(strict_types=1);

namespace LORIS\biobank\Endpoints;

use LORIS\biobank\Endpoint;

class Options extends Endpoint
{
    public function __construct(\User $user, \Database $db)
    {
        $this->db = $db;
        parent::__construct($user, null);  // Assuming no service is needed
    }

    protected function _hasAccess(string $permission) : bool
    {
        return true;
    }

    protected function allowedMethods() : array
    {
        return ['GET', 'OPTIONS'];
    }

    public function getFormattedAge($dob, $date) {
        if (!empty($dob) && !empty($date)) {
            $ageDetails = \Utility::calculateAge($dob, $date);
            return "{$ageDetails['year']} years";  // Assuming you want the year component formatted.
        }
        return null;  // or return 'Unknown' or any other default value you prefer
    }

    /**
     * Retrieves all options for populating forms and mapping front-end data.
     *
     * @param \Database $db   Database instance
     * @param \User     $user User instance
     *
     * @return array All options required by the Biobank Module
     */
    private function _handleGET(
        ServerRequestInterface $request
    ): ResponseInterface
    {
        $containerService = new ContainerService(new ContainerDAO($this->db));
        $specimenSergice = new SpecimenService(new SpecimenDAO($this->db));
        $shipmentDAO  = new ShipmentDAO($this->db);

        $query     = 'SELECT Name FROM diagnosis';
        $diagnoses = $this->db->pselectCol($query, []);

        $query     = 'SELECT full_name FROM examiners';
        $examiners = $this->db->pselectCol($query, []);

        $query = 'SELECT Real_name FROM users';
        $users = $this->db->pselectCol($query, []);

        $query = 'SELECT Name FROM psc';
        $centers = $this->db->pselectCol($query, []);

        $query = 'SELECT Name FROM Project';
        $projects = $this->db->pselectCol($query, []);

        $userCenters = implode(',',$this->user->getCenterIDs());
        $userProjects = implode(',', $this->user->getProjectIDs());

        $query      = "SELECT
                            c.CandID as id,
                            c.PSCID as pscid,
                            c.Sex as sex,
                            c.DoB as dob,
                            GROUP_CONCAT(DISTINCT d.Name) as diagnosis
                       FROM candidate c
                            INNER JOIN session s USING (CandID)
                            LEFT JOIN candidate_diagnosis_rel USING (CandID)
                            LEFT JOIN diagnosis d USING (DiagnosisID)
                            INNER JOIN biobank_specimen bs ON s.ID=bs.SessionID
                            LEFT JOIN psc USING (CenterID)
                       WHERE s.CenterID IN ($userCenters) OR s.ProjectID IN ($userProjects)
                       GROUP BY CandID";
        $candidates = $db->pselectWithIndexKey($query, [], 'pscid');

        foreach ($candidates as $pscid => $candidate) {
            $candidateIds[] = $candidate['id'];  // Collect candidate IDs for the second query
            $candidates[$pscid]['diagnosis'] = explode(',', $candidate['diagnosis']);
        }

        // Prepare PSCIDs for SQL IN clause
        // Extracting PSCIDs and preparing them as a comma-separated list of quoted strings
        $candidatePSCIDs = implode(',', array_map(function($pscid) {
            return "'" . $pscid . "'";
        }, array_keys($candidates)));
        $query = "SELECT
                   c.PSCID as pscid,
                   s.Visit_label as visit,
                   s.Date_visit as date,
                   psc.Name as center
                 FROM session s
                 LEFT JOIN psc ON s.CenterID = psc.CenterID
                 INNER JOIN candidate c ON c.CandID=s.CandID
                 INNER JOIN biobank_specimen bs ON s.ID=bs.SessionID
                 WHERE c.PSCID IN ($candidatePSCIDs)";
        $sessions = $db->pselect($query, []);

        foreach ($sessions as $session) {
            $candidateDob = $candidates[$pscid]['dob'] ?? null;
            $sessionDate = $session['date'] ?? null;
            $candidates[$session['pscid']]['sessions'][$session['visit']] = [
                'center' => $session['center'],
                'age' => $this->getFormattedAge($candidateDob, $sessionDate)
            ];
        }

        // foreach ($centers as $id => $center) {
        //     if ($user->hasCenter(new \CenterID(strval($id))) === false) {
        //         unset($centers[$id]);
        //     }
        // }

        $shipment = [
            'statuses' => $shipmentDAO->getStatuses(),
            'types'    => $shipmentDAO->getTypes(),
        ];

        $fields = [
            'diagnoses' => $diagnoses,
            'projects'  => $projects,
            'centers'   => $centers,
            'examiners' => $examiners,
            'users'     => $users,
            'container' => $containerService->getFields(),
            'shipment'  => $shipment,
            'specimen'  => $specimenService->getFields(),
        ]

        $configuration = [
        ];

        return [
            'fields'     => $fields,
            'config'     =>
            'candidates' => $candidates,
            'specimen'   => $specimenService->getOptions(),
        ];
    }
}
