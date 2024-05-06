<?php declare(strict_types=1);
/**
 * Pool Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Pool Object instances
 *
 * PHP Version 7.2
 *
 * @category   DAO
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */

namespace LORIS\biobank;

/**
 * Pool Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Pool Object instances
 *
 * PHP Version 7.2
 *
 * @category   DAO
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */

class PoolDAO extends \LORIS\Data\ProvisionerInstance
{
    /**
     * Private instance of the Database class.
     *
     * @var \LORIS\LorisInstance $loris
     * @var \Database $db
     */
    private $loris;
    private $db;

    /**
     * Initializes a new instance of the PoolDAO Class
     *
     * @param \LORIS\LorisInstance $loris LORIS instance object
     */
    function __construct(\LORIS\LorisInstance $loris)
    {
        $this->loris = $loris;
        $this->db    = $loris->getDatabaseConnection();
    }

    /**
     * This will load all pool persistence variables from the database for a
     * given $id into a Pool object, and return the object.
     *
     * @param int $id Value of the $id for the Pool that will be
     *                instantiated.
     *
     * @return Pool $pool Pool Instance.
     */
    public function getInstanceById(string $id) : Pool
    {
        return $this->selectInstances([
            ['column' => 'bp.PoolLabel', 'value'=>$id]
        ])[$id];
    }

    // XXX: Currently this function is never used with any conditions passed as
    // paramters. Decide if this is a necessary feature.
    /**
     * This will select all pools from the database that match the
     * attribute values passed through $poolData and will return an array
     * of pool objects.
     *
     * @param ?array $conditions Conditions to be met by the query
     * @param string $operator   Operator to link conditions with
     *
     * @return array $pools      List of Pool Objects that match the query
     */
    public function selectInstances(
        ?array $conditions = [],
        ?int $limit = null,
        int $offset = 0,
    ) : array {
        $query = "SELECT bp.Label as label,
                         bp.Quantity as quantity,
                         bu.Label as unit,
                         GROUP_CONCAT(bc.Barcode) as specimens,
                         bp.Date as date,
                         DATE_FORMAT(bp.Time, '%H:%i') as time
                  FROM   biobank_pool bp
                  LEFT JOIN
                         biobank_unit bu ON
                         bu.UnitID=bp.UnitID
                  LEFT JOIN
                         biobank_specimen_pool_rel bs_bp ON
                         bs_bp.PoolID=bp.PoolID
                  LEFT JOIN
                         biobank_specimen bs ON
                         bs.SpecimenID=bs_bp.SpecimenID
                  LEFT JOIN
                         biobank_container bc ON
                         bc.ContainerID=bs.ContainerID";

        $groupBy = 'bp.PoolId';                                            

        $result = DAOUtility::buildQuery($query, $conditions, $groupBy, $limit, $offset);
        $query = $result['query'];                                              
        $params = $result['params'];                                       

        $poolRows = $this->db->pselectWithIndexKey($query, [], 'label');

        $pools       = [];
        $specimenDAO = new SpecimenDAO($this->loris);
        
        foreach ($poolRows as $id => $poolRow) {
            $specimens = explode(',', $poolRow['specimens']);

            // set global pool properties based on first specimen of pool
            $specimen = $specimenDAO->getInstanceById($specimens[0]);
            // error_log(json_encode($specimen, JSON_PRETTY_PRINT));
            $poolRow['specimens'] = $specimens;
            $poolRow['candidate'] = $specimen->getCandidate();
            $poolRow['session']   = $specimen->getSession();
            $poolRow['center']    = $specimen->getCenter();
            $poolRow['projects']  = $specimen->getProjects();
            $poolRow['type']      = $specimen->getType();

            // instantiate pool entity
            $pools[$id] = new Pool($poolRow);
        }

        return $pools;
    }

    /**
     * Instantiates an ArrayIterator class that is composed of all the Pool
     * Objects
     *
     * @return \Traversable Iterator of Pool Objects
     */
    protected function getAllInstances() : \Traversable
    {
        return new \ArrayIterator($this->selectInstances());
    }

    /**
     * This function recieves a Pool Object, converts it into an SQL format
     * and inserts it into all relevant tables in the database. Certain
     * information is then cascaded to the children of the Pool.
     *
     * @param Pool $pool The Pool to be inserted into the database.
     *
     * @return Pool The pool that was inserted into the database
     */
    public function saveInstance(Pool $pool) : Pool
    {
        //Convert Pool to SQL format.
        $data = $this->_getSQLFromInstance($pool);

        $this->db->insertOnDuplicateUpdate(
            'biobank_pool',
            $data['biobank_pool']
        );

        $poolId = $pool->getId();
        if (isset($poolId)) {
            // If update:
            // Do nothing.
        } else {
            // If insert:
            $pool->setId((int) $this->db->getLastInsertId());
            // set the respective data array index to the value of the new
            // pool ID
            $specimenIds = $pool->getSpecimenIds();
            foreach ($specimenIds as $id) {
                $data['biobank_specimen_pool_rel'][$id]['PoolID'] = $pool->getId();
            }
        }

        // insert or update biobank_specimen_pool_rel with relevant data.
        foreach ($data['biobank_specimen_pool_rel'] as $insert) {
            $this->db->insertOnDuplicateUpdate(
                'biobank_specimen_pool_rel',
                $insert
            );
        }

        // XXX: decide if quantity of all pooled specimens should go to 0.
        // XXX: logic for adding preparations to pools will likely go here as well.

        return $this->getInstanceFromId($pool->getId());
    }

    /**
     * This function takes a Pool array and prepares the data to be inserted
     * into the database by converting it to a data array. This mapping is done
     * to enable the update or insertion of data into the database directly from
     * the resuling array.
     *
     * @param Pool $pool Pool array to be converted
     *
     * @return array $data Array containing the data to be inserted
     */
    private function _getSQLFromInstance(Pool $pool) : array
    {
        $poolData = [
            'PoolID'   => $pool->getId(),
            'Label'    => $pool->getLabel(),
            'Quantity' => $pool->getQuantity(),
            'UnitID'   => $pool->getUnitId(),
            'Date'     => $pool->getDate()->format('Y-m-d'),
            'Time'     => $pool->getTime()->format('H:i'),
        ];

        $specimenIds = $pool->getSpecimenIds();
        foreach ($specimenIds as $id) {
            $specimenPoolData[$id] = [
                'PoolID'     => $pool->getId(),
                'SpecimenID' => $id,
            ];
        }

        return [
            'biobank_pool'              => $poolData,
            'biobank_specimen_pool_rel' => $specimenPoolData ?? '',
        ];
    }
}
