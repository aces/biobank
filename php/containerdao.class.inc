<?php declare(strict_types=1);
/**
 * Container Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Container Object instances
 *
 * PHP Version 7.2
 *
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
namespace LORIS\biobank;

/**
 * Container Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Container Object instances
 *
 * PHP Version 7.2
 *
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class ContainerDAO extends \LORIS\Data\ProvisionerInstance
{
    /**
     * Private instance of the Database class.
     *
     * @var \Database $dao
     */
    private $db;

    /**
     * Initializes a new instance of the ContainerDAO Class
     *
     * @param \Database $db Instance of the LORIS Database class
     */
    function __construct(\Database $db)
    {
        $this->db = $db;
    }

    // XXX: I'm not sure if this function is really necessary since it is only
    // being used once, and should likely not be accessed outside this class.
    /**
     * This method is used when the DAO class needs to create a new Container
     * instance.
     *
     * @return Container
     */
    public function getInstance() : Container
    {
        return new Container();
    }

    /**
     * This will load all container persistence variables from the database
     * for a given $id into a Container object, and return the object.
     *
     * @param int $id Value of the $id for the Container that will be
     *                instantiated.
     *
     * @return ?Container $container Container Instance.
     */
    public function getInstanceFromId(int $id) : ?Container
    {
        return $this->selectInstances(
            [
                ['column'=>'bc.ContainerID', 'value'=>$id],
            ],
        )[$id] ?? null;
    }

    /**
     * Returns an array of all the Child Container IDs associated with
     * the given Container ID from the biobank_container_parent table.
     *
     * @param ?int $id of Container
     *
     * @return array $childContainerIds List of Container IDs that are
     *                                  children of the Container ID
     */
    private function _getChildContainerIds(?int $id) : array
    {
         $query = 'SELECT ContainerID
                   FROM biobank_container_parent
                   WHERE ParentContainerID=:i';
         $childContainerIds = $this->db->pselectCol($query, ['i' => $id]);

         return $childContainerIds;
    }

    /**
     * Returns an array of all the Project IDs associated with the given
     * Container ID from the biobank_container_project_rel table.
     *
     * @param ?int $id of Container
     *
     * @return $projectIds List of Project Ids that are associated with the
     *                     Container ID.
     */
    private function _getProjectIds(?int $id) : array
    {
        $query      = 'SELECT ProjectID
                  FROM biobank_container_project_rel
                  WHERE ContainerID=:i';
        $projectIds = $this->db->pselectCol($query, ['i' => $id]);

        return $projectIds;
    }

    // XXX: Currently this function is never used with any conditions passed as
    // parameters. Decide if this is a necessary feature. It is likely useful,
    // but I need to figure out how to integrate it wit the dataprovisioner.
    /**
     * This will select all containers from the database that match the
     * attribute values passed by $conditions and will return an array
     * of container objects.
     *
     * @param ?array $conditions Conditions to be met by the query
     * @param string $operator   The operator to link the conditions
     *
     * @return array $containers List of Container Objects that match the query
     */
    public function selectInstances(
        ?array $conditions = [],
        string $operator = 'AND'
    ) : array {
        $query = 'SELECT bc.ContainerID,
                     bc.Barcode,
                     bc.ContainerTypeID,
                     bs.SpecimenID,
                     bct.ContainerCapacityID,
                     bct.ContainerDimensionID,
                     bc.ContainerStatusID,
                     GROUP_CONCAT(DISTINCT(bcpr.ProjectID)) as ProjectIDs,
                     GROUP_CONCAT(DISTINCT(s.Barcode)) as ShipmentBarcodes,
                     bc.Temperature,
                     bc.CenterID,
                     bcp.ParentContainerID,
                     GROUP_CONCAT(DISTINCT(bcp2.ContainerID)) as ChildContainerIDs,
                     bcp.Coordinate as Coordinate,
                     bc.LotNumber,
                     bc.ExpirationDate,
                     bc.Comments,
                     bc.DateTimeUpdate,
                     bc.DateTimeCreate
                  FROM biobank_container bc
                  LEFT JOIN biobank_container_parent bcp
                    ON bc.ContainerID=bcp.ContainerID
                  LEFT JOIN biobank_container_parent bcp2
                    ON bc.ContainerID=bcp2.ParentContainerID
                  LEFT JOIN biobank_container_type bct
                    USING (ContainerTypeID)
                  LEFT JOIN biobank_container_project_rel bcpr
                    ON bc.ContainerID=bcpr.ContainerID
                  LEFT JOIN biobank_container_shipment_rel bcsr
                    ON bc.ContainerID=bcsr.ContainerID
                  LEFT JOIN shipment s
                    ON bcsr.ShipmentID=s.ShipmentID
                  LEFT JOIN biobank_specimen bs
                    ON bc.ContainerID=bs.ContainerID';
        if (!empty($conditions)) {
            $whereClause = [];
            foreach ($conditions as $condition) {
                $whereClause[] = $condition['column']
                    . '='
                    . '"'
                    . $condition['value']
                    . '"';
            }
            $query .= ' WHERE '.implode(" $operator ", $whereClause);
        }
        $query        .= ' GROUP BY bc.ContainerID';
        $containerRows = $this->db->pselectWithIndexKey($query, [], 'ContainerID');

        $containers = [];
        foreach ($containerRows as $id => $containerRow) {
            // TODO: This whole section needs to be redone more like
            //       the shipment module
            $childContainerIds = $containerRow['ChildContainerIDs']
                ? explode(',', $containerRow['ChildContainerIDs'])
                : [];
            $projectIds        = explode(',', $containerRow['ProjectIDs']);
            $shipmentBarcodes  = $containerRow['ShipmentBarcodes']
                ? explode(',', $containerRow['ShipmentBarcodes'])
                : [];
            $containers[$id]   = $this->_getInstanceFromSQL(
                $containerRow,
                $childContainerIds,
                $projectIds,
                $shipmentBarcodes
            );
        }

        return $containers;
    }

    /**
     * Instantiates an ArrayIterator class that is composed of all the Container
     * Objects.
     *
     * @return \Traversable Iterator of Container Objects
     */
    protected function getAllInstances() : \Traversable
    {
        return new \ArrayIterator($this->selectInstances());
    }

    // XXX: The conditions parameter is only used to differentiate primary types
    // from non primary types. Decide if this is a front-end concern.
    /**
     * Queries all rows of the biobank_container_type table and returns a nested
     * array with the ID field as the index. Conditions may be passed to filter
     * the results
     *
     * @param ?array $conditions Conditions to be met by the query
     *
     * @return array $containerTypes All data concerning each container type
     */
    public function getTypes(?array $conditions = null) : array
    {
        $query = 'SELECT ContainerTypeID as id,
                          Brand as brand,
                          ProductNumber as productNumber,
                          Label as label,
                          `Primary` as `primary`,
                          ContainerCapacityID as capacityId,
                          ContainerDimensionID as dimensionId
                   FROM biobank_container_type';
        if ($conditions != null) {
            $query .= $conditions
                ? ' WHERE ' . $this->db->_implodeWithKeys(' AND ', $conditions)
                : null;
        }
        $types = $this->db->pselectWithIndexKey($query, [], 'id');

        return $types;
    }

    // XXX: Althought container capacities were at first thought to be important
    // in the design of this entity, they are not being used in any way
    // throughout the module. Decide if container capacities are still
    // important, or if they should be deprecated.
    /**
     * Queries all rows of the biobank_container_capacity table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $containerCapacities All data concerning each container
     *                                    capacity
     */
    public function getCapacities() : array
    {
        $query      = 'SELECT ContainerCapacityID as id,
                          Quantity as quantity,
                          UnitID as unitId 
                  FROM biobank_container_capacity';
        $capacities = $this->db->pselectWithIndexKey($query, [], 'id');

        return $capacities;
    }

    // XXX: Although container units were at first thought to be important
    // in the design of this entity, they are only being used by Specimen Objects
    // in this module. Decide if container units are still important, or if they
    // should be deprecated.
    /**
     * Queries all rows of the biobank_unit table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $containerUnits All data concerning each unit
     */
    public function getUnits() : array
    {
        $query = "SELECT UnitID as id,
                         Label as unit 
                  FROM biobank_unit";
        $units = $this->db->pselectWithIndexKey($query, [], 'id');

        return $units;
    }

    /**
     * Queries all rows of the biobank_container_dimension table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $containerDimensions All data concerning each container
     *                                    dimension
     */
    public function getDimensions() : array
    {
        $query = "SELECT ContainerDimensionID as id,
                         X as x,
                         XNumerical as xNum,
                         Y as y,
                         YNumerical as yNum,
                         Z as z,
                         ZNumerical as zNum
                  FROM biobank_container_dimension";
        return $this->db->pselectWithIndexKey($query, [], 'id');
    }

    /**
     * Queries all rows of the biobank_container_status table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $containerStati All data concerning each container status
     */
    public function getStati() : array
    {
        $query = "SELECT ContainerStatusID as id,
                         Label as label
                  FROM biobank_container_status";
        $stati = $this->db->pselectWithIndexKey($query, [], 'id');

        return $stati;
    }

    /**
     * Queries all rows of the biobank_container_parent table and returns a
     * nested array with the Parent Container ID field as the first index, the
     * coordinate value as the second index, and the Child Container ID as the
     * value.
     *
     * If there is no coordinate, the string 'Unassigned' serves as the second
     * index.
     *
     * @return array $coordinates A Nested array of Child Container IDs
     *                            indexed by Parent Container ID and
     *                            Coordinate value or Unassigned.
     */
    public function getCoordinates() : array
    {
        $query       = "SELECT ContainerID as childContainerId,
                         ParentContainerID as parentContainerId,
                         Coordinate as coordinate
                        FROM biobank_container_parent";
        $result      = $this->db->pselect($query, []);
        $coordinates = [];
        foreach ($result as $row) {
            if (!isset($coordinates[$row['parentContainerId']])) {
                    $coordinates[$row['parentContainerId']] = [];
            };
            $container =& $coordinates[$row['parentContainerId']];
            if (empty($row['coordinate'])) {
                $container['Unassigned'][] = $row['childContainerId'];
            } else {
                $container[$row['coordinate']] = $row['childContainerId'];
            }
        }

        return $coordinates;
    }

    /**
     * This function receives a Container Object, converts it into a SQL format
     * and inserts it into all relevant tables in the database. Certain
     * information is then cascaded to the children of the Container.
     *
     * @param Container $container The Container Object to be saved into the
     *                             database.
     *
     * @return Container $container The Container Object that was saved to the
                                     database.
     */
    public function createInstance(Container $container) : Container
    {
        // TODO: THIS IS DUPLICATED IN BOTH FUNCTIONS AND COULD PROBABLY BE
        // DEALT WITH EITHER FRONT END OR SOMEWHERE ELSE.
        $parentContainerId = $container->getParentContainerId();
        if ($parentContainerId) {
            $parentContainer = $this->getInstanceFromId($parentContainerId);
            $container->setTemperature($parentContainer->getTemperature());
        }

        $data = $this->_getSQLFromInstance($container);
        // Inserts or Updates the biobank_container table with given data.
        $this->db->insertOnDuplicateUpdate(
            'biobank_container',
            $data['biobank_container']
        );

        $container->setId((int) $this->db->getLastInsertId());
        // set the respective data array index to the value of the new
        // container ID.
        $data['biobank_container_parent']['ContainerID'] = $container->getId();
        foreach ($data['biobank_container_project_rel'] as $id => $insert) {
            $projdata =& $data['biobank_container_project_rel'][$id];
            $projdata['ContainerID'] = $container->getId();
        }
        $this->saveParentContainer($container, $data);
        $this->_saveProject($container, $data);

        return $this->getInstanceFromId($container->getId());
    }

    /**
     * Update a container on and save on the LORIS SQL database
     *
     * @param Container $container the container to save
     *
     * @return array
     */
    public function updateInstance($container) : array
    {
        // TODO: THIS IS DUPLICATED IN BOTH FUNCTIONS AND COULD PROBABLY BE
        // DEALT WITH EITHER FRONT END OR SOMEWHERE ELSE.
        $parentContainerId = $container->getParentContainerId();
        if ($parentContainerId) {
            $parentContainer = $this->getInstanceFromId($parentContainerId);
            $container->setTemperature($parentContainer->getTemperature());
        }

        // Converts Container Object into SQL format.
        $oldContainer = $this->getInstanceFromId($container->getId());
        $data         = $this->_getSQLFromInstance($container);
        $this->db->insertOnDuplicateUpdate(
            'biobank_container',
            $data['biobank_container']
        );

        $updatedContainers = [$container];
        $this->saveParentContainer($container, $data);
        $this->_saveProject($container, $data);

        //Cascade changes in temperature, status, and center to all child Containers
        if ($oldContainer->getParentContainerId()) {
            $updatedContainers[] = $this->getInstanceFromId(
                $oldContainer->getParentContainerId()
            );
        }
        if ($container->getParentContainerId()) {
            $updatedContainers[] = $this->getInstanceFromId(
                $container->getParentContainerId()
            );
        }
        if ($container->getTemperature() !== $oldContainer->getTemperature()) {
            $updatedChildren   = $this->_cascadeToChildren(
                $container,
                'Temperature',
                $container->getTemperature()
            );
            $updatedContainers = array_merge(
                $updatedChildren,
                $updatedContainers
            );
        }
        if ($container->getStatusId() !== $oldContainer->getStatusId()) {
            $updatedChildren   = $this->_cascadeToChildren(
                $container,
                'ContainerStatusID',
                $container->getStatusId()
            );
            $updatedContainers = array_merge($updatedChildren, $updatedContainers);
        }
        if ($container->getCenterID() !== $oldContainer->getCenterID()) {
            $updatedChildren   = $this->_cascadeToChildren(
                $container,
                'CenterID',
                $container->getCenterID()
            );
            $updatedContainers = array_merge($updatedChildren, $updatedContainers);
        }

        return $updatedContainers;
    }

    /**
     * Save the parent hierarchy of a container to the database
     *
     * @param Container $container The child container
     * @param array     $data      The child's data
     *
     * @return void
     */
    public function saveParentContainer($container, $data)
    {
        // If there is no longer an associated Parent Container ID, delete
        // it from the entry from the biobank_container_parent table.
        $parentContainerId = $container->getParentContainerId();
        if (!isset($parentContainerId)) {
            $this->db->delete(
                'biobank_container_parent',
                ['ContainerID' => $container->getId()]
            );
        }

        // If parent container ID exists, create an association in the
        // biobank_container_parent rel table
        if (isset($parentContainerId)) {
            $this->db->insertOnDuplicateUpdate(
                'biobank_container_parent',
                $data['biobank_container_parent']
            );
        }
    }

    /**
     * Saves the container to the LORIS SQL database.
     *
     * @param Container $container The container to save
     * @param array     $data      The data to save
     *
     * @return void
     */
    private function _saveProject($container, $data)
    {
        // insert on update biobank_container_project_rel with relevant data.
        $this->db->delete(
            'biobank_container_project_rel',
            ['ContainerID' => $container->getId()]
        );
        foreach ($data['biobank_container_project_rel'] as $insert) {
            $this->db->insert(
                'biobank_container_project_rel',
                $insert
            );
        }
    }

    /**
     * This recursive function cascades the $value to the specified $field of
     * all the children of the $container Object.
     *
     * @param Container $container Container Object from which to retrieve children
     * @param string    $field     The field at which to associate the $value
     * @param mixed     $value     The value to assigned to the $field in question.
     *                             It is mixed since it may receive multiple types
     *                             of data.
     *
     * @return array            Array of all child container objects that were
     *                          updated.
     */
    private function _cascadeToChildren(
        Container $container,
        string $field,
        $value
    ) : array {
        $childContainerIds = $this->getAllChildContainerIds($container);
        if (empty($childContainerIds)) {
            return [];
        }
        $query = "UPDATE biobank_container                                      
                  SET $field=$value                                             
                  WHERE ContainerID IN (".implode(',', $childContainerIds).");";
        $this->db->run($query);
        $conditions = [];
        foreach ($childContainerIds as $id) {
            $conditions[] = ['column' => 'bc.ContainerID', 'value' => $id];
        }
        return $this->selectInstances($conditions, 'OR');
    }

    // FIXME: For some reason, this isn't working as intended and is returning
    // way more ChildContainers than necessary.
    /**
     * This function takes a Container Object and retrieves an array of ContainerIDs
     * representing all the descendant children of the Container.
     *
     * @param Container $container Container Object from which to retrieve
     *                             all children
     *
     * @return array  $childContainers Array of child Container Ids
     */
    public function getAllChildContainerIds(Container $container) : array
    {
        $query = 'WITH recursive cte (ContainerID) as 
                  (
                    SELECT ContainerID
                    FROM biobank_container_parent
                    WHERE ParentContainerID=:i
                    UNION ALL 
                    SELECT child.ContainerID
                    FROM biobank_container_parent as child
                    INNER JOIN cte on cte.ContainerID=child.ParentContainerID
                  )
                  SELECT ContainerID FROM cte';
        $childContainerIds = $this->db->pselectCol(
            $query,
            ['i' => $container->getId()]
        );

        return $childContainerIds;
    }

    // FIXME: Not in use. Delete if not used by November.
    /**
     * This function takes a Container Object and retrieves an array of objects
     * representing the childre of the $container.
     *
     * @param object $container Container Object from which to retrieve
     *                          children
     *
     * @return array  $childContainers Array of child container instances
     */
    // public function getChildContainers(Container $container) : array
    // {
    //     $childContainers   = array();
    //     $childContainerIds = $this->_getChildContainerIds($container->getId());
    //     if (!empty($childContainerIds)) {
    //         foreach ($childContainerIds as $id) {
    //             $childContainers[$id] = $this->getInstanceFromId((int) $id);
    //         }
    //     }
    //     return $childContainers;
    // }

    /**
     * This function takes a Container Object and prepares the data to be
     * inserted into the database by converting it to a data array. This mapping
     * is done to enable the update or insertion of data into the database
     * directly from the resulting arrays.
     *
     * @param Container $container Container Object to be converted
     *
     * @return array $data Array containing the data to be inserted
     */
    private function _getSQLFromInstance(Container $container) : array
    {
         $expirationDate = $container->getExpirationDate();
        if ($expirationDate) {
            $expirationDate = $expirationDate->format('Y-m-d');
        }
         $containerData = [
             'ContainerID'       => $container->getId(),
             'Barcode'           => $container->getBarcode(),
             'ContainerTypeID'   => $container->getTypeId(),
             'Temperature'       => $container->getTemperature(),
             'ContainerStatusID' => $container->getStatusId(),
             'CenterID'          => $container->getCenterID(),
             'LotNumber'         => $container->getLotNumber(),
             'ExpirationDate'    => $expirationDate,
             'Comments'          => $container->getComments(),
         ];

         $parentData = [
             'ContainerID'       => $container->getId(),
             'ParentContainerID' => $container->getParentContainerId(),
             'Coordinate'        => $container->getCoordinate(),
         ];

         $containerProjectData = [];

         foreach ($container->getProjectIds() as $id) {
             $containerProjectData[$id] = [
                 'ContainerID' => $container->getId(),
                 'ProjectID'   => $id,
             ];
         }

         return [
             'biobank_container'             => $containerData,
             'biobank_container_parent'      => $parentData,
             'biobank_container_project_rel' => $containerProjectData,
         ];
    }

    /**
     * This function takes an array that resulted from an SQL query and
     * instantiates it as a Container Object
     *
     * @param array  $data              Values to be instantiated.
     * @param ?array $childContainerIds List of child container ids.
     * @param ?array $projectIds        List of project ids.
     * @param ?array $shipmentBarcodes  List of barcodes.
     *
     * @return Container
     */
    private function _getInstanceFromSQL(
        array $data,
        ?array $childContainerIds,
        ?array $projectIds,
        ?array $shipmentBarcodes
    ) : Container {
        $container = $this->getInstance();

        if (isset($data['ContainerID'])) {
            $container->setId((int) $data['ContainerID']);
        }
        if (isset($data['Barcode'])) {
            $container->setBarcode((string) $data['Barcode']);
        }
        if (isset($data['SpecimenID'])) {
            $container->setSpecimenId((int) $data['SpecimenID']);
        }
        if (isset($data['ContainerTypeID'])) {
            $container->setTypeId((int) $data['ContainerTypeID']);
        }
        if (isset($data['ContainerDimensionID'])) {
            $container->setDimensionId((int) $data['ContainerDimensionID']);
        }
        if (isset($data['Temperature'])) {
            $container->setTemperature((float) $data['Temperature']);
        }
        if (isset($data['ContainerStatusID'])) {
            $container->setStatusId((int) $data['ContainerStatusID']);
        }
        if (isset($projectIds)) {
            $container->setProjectIds($projectIds);
        }
        if (isset($shipmentBarcodes)) {
            $container->setShipmentBarcodes($shipmentBarcodes);
        }
        if (isset($data['CenterID'])) {
            $container->setCenterId(new \CenterID(strval($data['CenterID'])));
        }
        if (isset($data['ParentContainerID'])) {
            $container->setParentContainerId((int) $data['ParentContainerID']);
        }
        if (isset($childContainerIds)) {
            $container->setChildContainerIds($childContainerIds);
        }
        if (isset($data['Coordinate'])) {
            $container->setCoordinate((int) $data['Coordinate']);
        }
        if (isset($data['LotNumber'])) {
            $container->setLotNumber((string) $data['LotNumber']);
        }
        if (isset($data['ExpirationDate'])) {
            $container->setExpirationDate(new \DateTime($data['ExpirationDate']));
        }
        if (isset($data['Comments'])) {
            $container->setComments((string) $data['Comments']);
        }

        return $container;
    }
}
