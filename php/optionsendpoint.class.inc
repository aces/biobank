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
class OptionsEndpoint extends \NDB_Page implements RequestHandlerInterface
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
        $db   = $this->loris->getDatabaseConnection();
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
        }
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

        $userCenters = implode(',',$user->getCenterIDs());
        // XXX: This should eventually be replaced by a call directly to a
        // Candidate endpoint or Candidate controller that will be able to
        // provide Candidate Objects.
        $query      = "SELECT c.CandID as id,
                            PSCID as pscid,
                            Sex as sex,
                            GROUP_CONCAT(DISTINCT DiagnosisID) as diagnosisIds
                       FROM candidate c
                            LEFT JOIN session s USING (CandID)
                            LEFT JOIN biobank_specimen bs ON (s.Id = bs.SessionID)
                            LEFT JOIN biobank_container bc USING (ContainerID)
                            LEFT JOIN biobank_specimen_pool_rel bspr USING (SpecimenID)
                            LEFT JOIN biobank_pool bp USING (PoolID)
                            LEFT JOIN candidate_diagnosis_rel USING (CandID)";
        $where = ' ';
        if (!$user->hasPermission('access_all_profiles')) {
            $where = " WHERE s.CenterID IN ($userCenters)
                            OR bc.CenterID IN ($userCenters)
                            OR bp.CenterID IN ($userCenters)";
        }
        $group      =   " GROUP BY CandID";
        $candidates = $db->pselectWithIndexKey($query.$where.$group, array(), 'id');
        foreach ($candidates as $id => $candidate) {
            $candidate['diagnosisIds'] = $candidate['diagnosisIds']
                ? explode(',', $candidate['diagnosisIds'])
                : [];
            $candidates[$id]           = $candidate;
        }

        // XXX: This should eventually be replaced by a call directly to a
        // Candidate endpoint or Candidate controller that will be able to provide
        // Diagnosis Options.
        $query     = 'SELECT DiagnosisID as id, Name as label FROM diagnosis';
        $diagnoses = $db->pselectWithIndexKey($query, [], 'id');

        // XXX: This should eventually be replaced by a call directly to a
        // Session endpoint or Session controller that will be able to provide
        // Session Objects.
        $query    = 'SELECT ID as id, Visit_label as label FROM session';
        $sessions = $db->pselectWithIndexKey($query, [], 'id');

        // XXX: This should eventually be replaced by a call directly to a
        // Examiner endpoint or Examiner controller that will be able to provide
        // Examiner Objects.
        $query     = 'SELECT examinerID as id, full_name as label FROM examiners';
        $examiners = $db->pselectWithIndexKey($query, [], 'id');

        // XXX: This should eventually be replaced by a call directly to a
        // User endpoint or Examiner controller that will be able to provide
        // User Objects.
        $query = 'SELECT ID as id, Real_name as label FROM users';
        $users = $db->pselectWithIndexKey($query, [], 'id');

        // XXX: This should eventually be replaced by a call directly to a
        // Center Endpoint or Center Controller that will be able to provide
        // Center Objects.
        $centers = \Utility::getSiteList();
        foreach ($centers as $id => $center) {
            if ($user->hasCenter(new \CenterID(strval($id))) === false) {
                unset($centers[$id]);
            }
        }

        // XXX: This should eventually be replaced by a call directly to a
        // Session Controller or Session Options endpoint that will be able to
        // provide these options.
        $query  = "SELECT c.CandID as candidateId,
                         s.ID sessionId,
                         s.Visit_label as label,
                         s.CenterID as centerId
                 FROM candidate c
                        LEFT JOIN session s USING(CandID)
                        LEFT JOIN biobank_specimen bs ON (s.Id = bs.SessionID)
                        LEFT JOIN biobank_container bc USING (ContainerID)
                        LEFT JOIN biobank_specimen_pool_rel bspr USING (SpecimenID)
                        LEFT JOIN biobank_pool bp USING (PoolID)";
        $where = ' ';
        if (!$user->hasPermission('access_all_profiles')) {
            $where = " WHERE s.CenterID IN ($userCenters)
                            OR bc.CenterID IN ($userCenters)
                            OR bp.CenterID IN ($userCenters)";
        }
        $result = $db->pselect($query.$where, array());
        $candidateSessions = [];
        $sessionCenters    = [];
        foreach ($result as $row) {
            if (!isset($candidateSessions[$row['candidateId']][$row['sessionId']])) {
                    $candidateSessions[$row['candidateId']][$row['sessionId']] = [];
            };
            $ses =& $candidateSessions[$row['candidateId']][$row['sessionId']];
            $ses['id'] = $row['sessionId'];
            $ses['label'] = $row['label'];

            $sessionCenters[$row['sessionId']]['centerId'] = $row['centerId'];
        }

        $shipment = [
            'statuses' => $shipDAO->getStatuses(),
            'types'    => $shipDAO->getTypes(),
        ];

        return [
            'candidates'        => $candidates,
            'diagnoses'         => $diagnoses,
            'sessions'          => $sessions,
            'projects'          => \Utility::getProjectList(),
            'centers'           => $centers,
            'examiners'         => $examiners,
            'users'             => $users,
            'candidateSessions' => $candidateSessions,
            'sessionCenters'    => $sessionCenters,
            'container'         => $contCont->getOptions(),
            'specimen'          => $specCont->getOptions(),
            'shipment'          => $shipment,
        ];
    }
}
