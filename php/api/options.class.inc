<?php declare(strict_types=1);
/**
 * This implements the Options endpoint class for the Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   API
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris
 */
namespace LORIS\biobank;
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Server\RequestHandlerInterface;
use \Psr\Http\Message\ResponseInterface;

/**
 * A class for handling the /biobank/options/ endpoint.
 *
 * PHP Version 7.2
 *
 * @category   API
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
class Options extends \NDB_Page implements RequestHandlerInterface
{
    /**
     * Returns true if user has access to this endpoint.
     *
     * @param \User $user The user whose access is being checked.
     *
     * @return bool
     */
    function _hasAccess(\User $user) : bool
    {
        return true;
    }

    /**
     * Return which methods are supported by this endpoint.
     *
     * @return array supported HTTP methods
     */
    protected function allowedMethods() : array
    {
        return [
            'GET',
            'OPTIONS',
        ];
    }

    /**
     * This function can be overridden in a module's page to load the necessary
     * resources to check the permissions of a user.
     *
     * @param User                   $user    The user to load the resources for
     * @param ServerRequestInterface $request The PSR15 Request being handled
     *
     * @return void
     */
    public function loadResources(
        \User $user, ServerRequestInterface $request
    ) : void {
    }

    /**
     * This function passes the request to the handler. This is necessary since
     * the Endpoint bypass the Module class.
     *
     * XXX: This function should be extracted to a parent class.
     *
     * @param ServerRequestInterface  $request The PSR7 request.
     * @param RequestHandlerInterface $handler The request handler.
     *
     * @return ResponseInterface The outgoing PSR7 response.
     */
    public function process(
        ServerRequestInterface $request,
        RequestHandlerInterface $handler
    ) : ResponseInterface {
        return $handler->handle($request);
    }

    /**
     * Handles endpoint requests.
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(ServerRequestInterface $request) : ResponseInterface
    {
        ini_set('memory_limit', '-1');
        $db   = \NDB_Factory::singleton()->database();
        $user = $request->getAttribute('user');

        try {
            switch ($request->getMethod()) {
            case 'GET':
                $options = $this->getOptions($db, $user);
                return new \LORIS\Http\Response\JSON\OK($options);
            case 'OPTIONS':
                return (new \LORIS\Http\Response())
                    ->withHeader('Allow', $this->allowedMethods());
            }
        } catch (Forbidden $e) {
            return new \LORIS\Http\Response\JSON\Forbidden($e->getMessage());
        } catch (\Exception $e) {
           return new \LORIS\Http\Response\JSON\InternalServerError(
               $e->getMessage()
           );
        }
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
    function getOptions(\Database $db, \User $user)
    {
        $contCont = new ContainerController($this->loris, $user);
        $specCont = new SpecimenController($this->loris, $user);
        $shipDAO  = new ShipmentDAO($db);

        $query     = 'SELECT Name FROM diagnosis';
        $diagnoses = $db->pselectCol($query, []);

        $query     = 'SELECT full_name FROM examiners';
        $examiners = $db->pselectCol($query, []);

        $query = 'SELECT Real_name FROM users';
        $users = $db->pselectCol($query, []);

        $query = 'SELECT Name FROM psc';
        $centers = $db->pselectCol($query, []);

        $query = 'SELECT Name FROM Project';
        $projects = $db->pselectCol($query, []);

        $userCenters = implode(',',$user->getCenterIDs());
        $userProjects = implode(',', $user->getProjectIDs());

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
            'statuses' => $shipDAO->getStatuses(),
            'types'    => $shipDAO->getTypes(),
        ];

        $fields = [
            'diagnoses' => $diagnoses,
            'projects'  => $projects,
            'centers'   => $centers,
            'examiners' => $examiners,
            'users'     => $users,
            'container' => $contCont->getFields(),
            'shipment'  => $shipment,
            'specimen'  => $specCont->getFields(),
        ]

        $configuration = [
        ];

        return [
            'fields'     => $fields,
            'config'     =>
            'candidates' => $candidates,
            'specimen'   => $specCont->getOptions(),
        ];
    }
}
