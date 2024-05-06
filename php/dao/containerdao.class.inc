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
class ContainerDAO extends DAO, \LORIS\Data\ProvisionerInstance
{
    private $db;

    /**
     * Initializes a new instance of the ContainerDAO Class
     *
     * @param \Database $db Instance of the LORIS Database class
     */
    function __construct(\Database $db)
    {
        parent::__contruct($db);
    }

     protected $fieldMapping = [
        'barcode' => 'bc.Barcode',
        'type' => 'bct.Label',
        'status' => 'bcs.Status',
        // map more fields as necessary
    ];

    /**
     * This will load all container persistence variables from the database
     * for a given $id into a Container object, and return the object.
     *
     * @param string $barcode Value of the $barcode for the Container that will be
     *                instantiated.
     *
     * @return ?Container $container Container Instance.
     */
    public function getById(string $id) : ?Container
    {
        return $this->select(
            [
                ['column'=>'bc.Barcode', 'value'=>$id],
            ],
        )[$id] ?? null;
    }

    /**
     * Instantiates an ArrayIterator class that is composed of all the NON-PRIMARY
     * Container Objects.
     *
     * @return \Traversable Iterator of Container Objects
     */
    protected function getAllInstances() : \Traversable
    {
        return new \ArrayIterator($this->select([
          ['column' => 'bct.Primary', 'value' => 0]
        ]));
    }

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
    public function select(
        ?array $conditions = [],
        ?int $limit = null,
        int $offset = 0,
    ) : array {
        // this is causing major issues and needs to be reworked.
        // GROUP_CONCAT(
        //   DISTINCT CONCAT(bc3.Barcode, '-', bc3.Coordinate)
        // ) as ChildContainerBarcodes,
        $query = 'SELECT    bc.ContainerID as ID
                            bc.Barcode as Barcode,
                            bct.Label as ContainerType,
                            bcd.X,
                            bcd.Y,
                            bcd.Z,
                            bcd.XNumerical as XNum,
                            bcd.YNumerical as YNum,
                            bcd.ZNumerical as ZNum,
                            GROUP_CONCAT(DISTINCT(s.Barcode)) as Shipments,
                            bc.Temperature,
                            psc.Name as Center,
                            bc2.Barcode as Parent,
                            GROUP_CONCAT(DISTINCT bc3.Barcode) as Children,
                            bcp.Coordinate as Coordinate,
                            bc.LotNumber,
                            bc.ExpirationDate,
                            bc.Comments,
                            bc.DateTimeUpdate,
                            bc.DateTimeCreate
                  FROM      biobank_container bc
                  LEFT JOIN psc ON
                            psc.CenterID=bc.CenterID
                  LEFT JOIN biobank_container_parent bcp ON
                            bc.ContainerID=bcp.ContainerID
                  LEFT JOIN biobank_container bc2 ON
                            bcp.ParentContainerID=bc2.ContainerID
                  LEFT JOIN biobank_container_parent bcp2 ON
                            bc.ContainerID=bcp2.ParentContainerID
                  LEFT JOIN biobank_container bc3 ON
                            bcp2.ContainerID=bc3.ContainerID
                  LEFT JOIN biobank_container_type bct ON
                            bc.ContainerTypeID=bct.ContainerTypeID
                  LEFT JOIN biobank_container_dimension bcd ON
                            bcd.ContainerDimensionID=bct.ContainerDimensionID
                  LEFT JOIN biobank_container_shipment_rel bcsr ON
                            bc.ContainerID=bcsr.ContainerID
                  LEFT JOIN shipment s ON
                            bcsr.ShipmentID=s.ShipmentID
                  LEFT JOIN biobank_specimen bs ON
                            bc.ContainerID=bs.ContainerID';

        $groupBy = 'bc.Barcode';
        $result = DAO::buildQuery($query, $conditions, $groupBy, $limit, $offset);
        $query = $result['query'];
        $params = $result['params'];

        $containerRows = $this->db->pselectWithIndexKey($query, $params, 'Barcode');

        $containers = [];
        foreach ($containerRows as $id => $containerRow) {
            $containers[$id] = $this->_getFromSQL($containerRow);
        }
        return $containers;
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
        $query = 'SELECT  Label as type FROM biobank_container_type';
        if ($conditions != null) {
            $query .= $conditions
                ? ' WHERE ' . $this->db->_implodeWithKeys(' AND ', $conditions)
                : null;
        }
        $types = $this->db->pselectCol($query, []);

        return $types;
    }

    /**
     * Queries all rows of the biobank_container_status table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $containerStati All data concerning each container status
     */
    public function getStati() : array
    {
        $query = "SELECT Label FROM biobank_container_status";
        $stati = $this->db->pselectCol($query, []);

        return $stati;
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
    public function create(Container $container) : Container
    {

        $data = $this->_getSQL($container);

        // Inserts or Updates the biobank_container table with given data.
        $this->db->insertOnDuplicateUpdate(
            'biobank_container',
            $data['biobank_container']
        );

        $container->setId((int) $this->db->getLastInsertId());

        // set the respective data array index to the value of the new
        // container ID.
        $data['biobank_container_parent']['ContainerID'] = $container->getId();
        $this->saveParent($container, $data);

        return $this->getById($container->getBarcode());
    }

    /**
     * Update a container on and save on the LORIS SQL database
     *
     * @param Container $container the container to save
     *
     * @return array
     */
    public function update(Container $container) : Container
    {
        // Converts Container Object into SQL format.
        $data = $this->_getSQL($container);
        $this->db->insertOnDuplicateUpdate(
            'biobank_container',
            $data['biobank_container']
        );

        $this->saveParent($container, $data);

        return $container;
    }

    /**
     * Save the parent hierarchy of a container to the database
     *
     * @param Container $container The child container
     * @param array     $data      The child's data
     *
     * @return void
     */
    public function saveParent($container, $data)
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
     * This function takes a Container Object and prepares the data to be
     * inserted into the database by converting it to a data array. This mapping
     * is done to enable the update or insertion of data into the database
     * directly from the resulting arrays.
     *
     * @param Container $container Container Object to be converted
     *
     * @return array $data Array containing the data to be inserted
     */
    private function _getSQL(Container $container) : array
    {
         $containerData = [
             'ContainerID'    => $container->getId(),
             'Barcode'        => $container->getBarcode(),
             'ContainerTypeID'=> $container->getCenterId(),
             'Temperature'    => $container->getTemperature(),
             'CenterID'           => $container->getCenter(),
             'LotNumber'      => $container->getLotNumber(),
             'ExpirationDate' => $container->getExpirationDate()->format('Y-m-d'),
             'Comments'       => $container->getComments(),
         ];

         $parentData = [
             'ContainerID'       => $container->getId(),
             'ParentContainerID' => $container->getParentContainerId(),
             'Coordinate'        => $container->getCoordinate(),
         ];

         return [
             'biobank_container'             => $containerData,
             'biobank_container_parent'      => $parentData,
         ];
    }

    private function getCenterIdByLabel(Container $container) {
        getIdByLabel($container->getType(), 'Label', 'CenterID', 'psc');
    }

    private function getTypeIdByLabel($container) {
        getIdByLabel($container->getType(), 'Label', 'CenterID', 'psc');
    }

    /**
     * This function takes an array that resulted from an SQL query and
     * instantiates it as a Container Object
     *
     * @param array  $data              Values to be instantiated.
     * @param ?array $childContainerIds List of child container ids.
     * @param ?array $shipmentBarcodes  List of barcodes.
     *
     * @return Container
     */
    private function _getFromSQL(
        array $data,
    ) : Container {
        $container = new Container();

        $container->setBarcode((string) $data['Barcode']);
        $container->setType((string) $data['ContainerType'], );
        $container->setDimension([
            'X' => $data['X'],
            'Y' => $data['Y'],
            'Z' => $data['Z'],
            'XNum' => $data['XNum'],
            'YNum' => $data['YNum'],
            'ZNum' => $data['ZNum']
        ]);
        $container->setTemperature((float) $data['Temperature']);
        $container->setStatus((string) $data['ContainerStatus']);
        $container->setShipments(DAO::toArray($data['Shipments']);
        $container->setCenter($data['Center']);
        $container->setParent((string) $data['Parent']);
        $container->setChild(self::prepareChildren($data['Children']));
        $container->setCoordinate((int) $data['Coordinate']);
        $container->setLotNumber((string) $data['LotNumber']);
        $container->setExpirationDate(new \DateTime($data['ExpirationDate']));
        $container->setComments((string) $data['Comments']);

        return $container;
    }

    private function _prepareChildren(?array $data) {
        if ($childtaren) {
            $children = DAO::toArray($data);
            $indexedChildren = [];

            foreach ($children as $barcodeCoordinate) {
                list($barcode, $coordinate) = array_pad(explode('-', $barcodeCoordinate, 2), 2, null);
                // Check for a non-empty coordinate; treat empty or 'NULL' as unassigned
                if (!empty($coodinate) && strtolower($coordinate) !== 'null') {
                    // Child container is assigned a coordinate
                    $indexedChildren[$coordinate] = $barcode;
                } else {
                    // Child container is unassigned a coordinate
                    // Ensure 'unassigned' key exists as an array before appending
                    if (!isset($indexedChildren['unassigned'])) {
                        $indexedChildren['unassigned'] = [];
                    }
                    $indexedChildren['unassigned'][] = $barcode; // Add to unassigned array
                }
            }
            return $indexedChildren);
        } else {
            return [];
        }
    }
}
