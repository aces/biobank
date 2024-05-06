<?php declare(strict_types=1);
/**
 * Specimen Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Specimen Object instances
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
 * Specimen Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Specimen Object instances
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
class SpecimenDAO extends \LORIS\Data\ProvisionerInstance
{
    /**
     * The LORIS instance
     *
     * @var \LORIS\LORISInstance $loris
     * @var \Database $db
     */
    private $loris;
    private $db;

    /**
     * Initializes a new instance of the SpecimenDAO Class
     *
     * @param \LORIS\LorisInstance $loris Instance of the LORIS Database class
     */
    function __construct(\LORIS\LORISInstance $loris)
    {
        $this->loris = $loris;
        $this->db    = $loris->getDatabaseConnection();
    }

    /**
     * This will load all specimen persistence variables from the database for a
     * given $id into a Specimen object, and return the object.
     *
     * @param int $id Value of the $id for the Specimen that will be
     *                instantiated
     *
     * @return ?Specimen $specimen Specimen Instance.
     */
    public function getInstanceById(string $id) : ?Specimen
    {
        return $this->selectInstances([
            ['column'=>'bc.Barcode', 'value'=>$id]
        ])[$id] ?? null;
    }

    /**
     * This function returns an array of all the Parent Specimen IDs that are
     * associated with the given Specimen ID in the biobank_specimen_parent
     * table.
     *
     * @param int $id of Specimen
     *
     * @return array $parentSpecimenIds List of Specimen IDs that are
     *                                  parents of the given Specimen ID
     */
    public function getParentSpecimenIdsFromId(int $id) : array
    {
        $query = 'SELECT ParentSpecimenID
                  FROM biobank_specimen_parent
                  WHERE SpecimenID=:i';
        return $this->db->pselectcol($query, ['i' => $id]);
    }

    /**                                                                         
     * Returns an array of all the Project IDs associated with the given        
     * Specimen from the biobank_specimen_project_rel table.               
     *                                                                          
     * @param ?int $id                                       
     *                                                                          
     * @return $projectIds 
     */                                                                         
    private function _getProjectIds(?int $id) : array                           
    {                                                                           
        $query      = 'SELECT ProjectID                                         
                  FROM biobank_specimen_project_rel                            
                  WHERE SpecimenID=:i';                                        
        $projectIds = $this->db->pselectCol($query, ['i' => $id]);              
                                                                                
        return $projectIds;                                                     
    }  

    /**
     * This will select all specimens from the database that match the
     * attribute values passed by $conditions and will return an array
     * of specimen objects.
     *
     * @param ?array $conditions Conditions to be met by the query
     * @param string $operator   Operator to link the conditions. "AND" or "OR".
     *
     * @return array $specimens List of Specimen Objects that match the query
     */
    public function selectInstances(
        ?array $conditions = [],
        ?int $limit = 10,
        int $offset = 0,
    ) : array {
        $query = "SELECT bc.Barcode,
                         bst.Label as SpecimenType,
                         bs.Quantity,
                         bu.Label as Unit,
                         psc.Name as Center,
                         GROUP_CONCAT(DISTINCT(p.Name)) as Projects,
                         bsf.FreezeThawCycle,
                         GROUP_CONCAT(DISTINCT bc2.Barcode) as Parents,
                         s.Visit_label as Session,
                         c.PSCID as Candidate,
                         bp.Label as Pool,
                         bs_pr_c.Label as CollectionProtocol,
                         bs_c.Quantity as CollectionQuantity,
                         bu_c.Label as CollectionUnit,
                         psc_c.Name as CollectionCenter,
                         ex_c.full_name as CollectionExaminer,
                         bs_c.Date as CollectionDate,
                         bs_c.Time as CollectionTime,
                         bs_c.Comments as CollectionComments,
                         bs_c.Data as CollectionData,
                         bs_pr_p.Label as PreparationProtocol,
                         psc_p.Name as CollectionCenter,
                         ex_p.full_name as CollectionExaminer,
                         bs_p.Date as PreparationDate,
                         bs_p.Time as PreparationTime,
                         bs_p.Comments as PreparationComments,
                         bs_p.Data as PreparationData,
                         bs_pr_a.Label as AnalysisProtocol,
                         psc_a.Name as CollectionCenter,
                         ex_a.full_name as CollectionExaminer,
                         bs_a.Date as AnalysisDate,
                         bs_a.Time as AnalysisTime,
                         bs_a.Comments as AnalysisComments,
                         bs_a.Data as AnalysisData
               FROM      biobank_specimen bs

               -- Specimen Joins
               INNER JOIN biobank_container bc ON
                         bs.ContainerID=bc.ContainerID
               LEFT JOIN biobank_specimen_freezethaw bsf ON
                         bs.SpecimenID=bsf.SpecimenID
               LEFT JOIN biobank_specimen_parent bs_bs ON
                         bs.SpecimenID=bs_bs.SpecimenID
               LEFT JOIN biobank_specimen bs2 ON
                         bs_bs.ParentSpecimenID=bs2.SpecimenID
               LEFT JOIN biobank_container bc2 ON
                         bs2.ContainerID=bc2.ContainerID
               LEFT JOIN biobank_specimen_type bst ON
                         bs.SpecimenTypeID=bst.SpecimenTypeID
               LEFT JOIN biobank_unit bu ON
                         bs.UnitID=bu.UnitID

               -- Project, Session, Candidate, Center Joins
               LEFT JOIN biobank_specimen_project_rel bs_proj ON
                         bs.SpecimenID=bs_proj.SpecimenID
               LEFT JOIN Project p ON
                         bs_proj.ProjectID=p.ProjectID
               LEFT JOIN session s ON
                         bs.SessionID=s.ID
               LEFT JOIN candidate c ON
                         s.CandID=c.CandID
               LEFT JOIN psc ON
                         bc.CenterID=psc.CenterID


               -- Pool Joins
               LEFT JOIN biobank_specimen_pool_rel bs_bp
                 ON bs.SpecimenID=bs_bp.SpecimenID
               LEFT JOIN biobank_pool bp
                 ON bs_bp.PoolID=bp.PoolID

               -- Collection Joins
               LEFT JOIN biobank_specimen_collection bs_c
                   ON bs.SpecimenID=bs_c.SpecimenID
               LEFT JOIN biobank_specimen_protocol bs_pr_c
                   ON bs_c.SpecimenProtocolID=bs_pr_c.SpecimenProtocolID
               LEFT JOIN psc psc_c
                   ON bs_c.CenterID = psc_c.CenterID
               LEFT JOIN biobank_unit bu_c
                   ON bs_c.UnitID = bu_c.UnitID
               LEFT JOIN examiners ex_c
                   ON bs_c.ExaminerID = ex_c.ExaminerID

               -- Preparation Joins
               LEFT JOIN biobank_specimen_preparation bs_p
                 ON bs.SpecimenID=bs_p.SpecimenID
               LEFT JOIN biobank_specimen_protocol bs_pr_p
                   ON bs_p.SpecimenProtocolID=bs_pr_p.SpecimenProtocolID
               LEFT JOIN psc psc_p ON bs_p.CenterID = psc_p.CenterID
               LEFT JOIN examiners ex_p ON bs_p.ExaminerID = ex_p.ExaminerID

               -- Analysis Joins
               LEFT JOIN biobank_specimen_analysis bs_a
                 ON bs.SpecimenID=bs_a.SpecimenID
               LEFT JOIN biobank_specimen_protocol bs_pr_a
                   ON bs_a.SpecimenProtocolID=bs_pr_a.SpecimenProtocolID
               LEFT JOIN psc psc_a ON bs_a.CenterID = psc_a.CenterID
               LEFT JOIN examiners ex_a ON bs_a.ExaminerID = ex_a.ExaminerID

";

        $groupBy = 'bc.Barcode';
        $result = DAOUtility::buildQuery($query, $conditions, $groupBy, $limit, $offset);
        $query = $result['query'];
        $params = $result['params'];

        $specimenRows = $this->db->pselectWithIndexKey($query, $params, 'Barcode');
        $specimens = [];
        // XXX: join container to specimen. In the future for efficieny, this can
        // potentially be added to the query. Balance between added code/increased speed.
        $containerDAO = new ContainerDAO($this->db);
        foreach ($specimenRows as $id => $specimenRow) {
            $specimen = $this->_getInstanceFromSQL($specimenRow);
            $container = array_values($containerDAO->selectInstances([                                                                   
                ['column' => 'bc.Barcode', 'value' => $specimen->getBarcode()],                    
            ]))[0];
            $specimen->setContainer($container);
            $specimens[$id] = $specimen;
        }
        
        return $specimens;
    }

    /**
     * Instantiates an ArrayIterator class that is composed of all the Specimen
     * Objects.
     *
     * @return \Traversable Iterator of Specimen Objects
     */
    protected function getAllInstances() : \Traversable
    {
        return new \ArrayIterator($this->selectInstances());
    }

    public function getTypes() : array
    {
        $query = 'SELECT Label FROM biobank_specimen_type';
        return $this->db->pselectCol($query, []);
    }

    public function getProtocols(): array {
        $query = "SELECT
                      p.Label as protocol,
                      t.Label as type,
                      pr.Label as process
                  FROM biobank_specimen_protocol p
                  LEFT JOIN biobank_specimen_type t ON p.SpecimenTypeID = t.SpecimenTypeID
                  LEFT JOIN biobank_specimen_process pr ON p.SpecimenProcessID = pr.SpecimenProcessID";
    
        $result = $this->db->pselect($query, []);
    
        $protocols = [];
        foreach ($result as $row) {
            // Initialize the protocol array if not already set
            if (!isset($protocols[$row['protocol']])) {
                $protocols[$row['protocol']] = [
                    'types' => [],
                    'processes' => []
                ];
            }
    
            // Avoid adding duplicate type or process labels
            if (!in_array($row['type'], $protocols[$row['protocol']]['types'])) {
                $protocols[$row['protocol']]['types'][] = $row['type'];
            }
            if (!in_array($row['process'], $protocols[$row['protocol']]['processes'])) {
                $protocols[$row['protocol']]['processes'][] = $row['process'];
            }
        }
    
        return $protocols;
    }

    /**
     * Queries all rows of the biobank_specimen_process table and returns a
     * nested array with the ID field as the index.
     *
     * @return array $processes All data concerning each specimen
     *                          process.
     */
    public function getProcesses() : array
    {
        $query     = "SELECT Label FROM biobank_specimen_process";
        $processes = $this->db->pselectCol($query, []);

        return $processes;
    }

    /**
     * Queries all rows of the biobank_specimen_attribute table and returns an
     * array with the ID field as the index
     *
     * @return array $attributes All data concerning each attribute
     */
    public function getAttributes() : array
    {
        $query      = 'SELECT bsa.Label as attribute,
                              bsad.Datatype as datatype
                  FROM biobank_specimen_attribute bsa
                  JOIN biobank_specimen_attribute_datatype bsad USING (DatatypeID)';
        $attributes = $this->db->pselectWithIndexKey($query, [], 'attribute');

        return $attributes;
    }

    public function getUnits() : array
    {
        $query = "SELECT Label FROM biobank_unit";
        return $this->db->pselectCol($query, []);
    }

    /**
     * Queries all rows of the biobank_specimen_type_unit_rel table and returns
     * a nested array of specimen unit values, with the Type ID as the first index,
     * and the Unit ID as the second index.
     *
     * @return array $typeUnits A nested array of unit values index by
     *                                  Type ID and Unit ID
     */
    public function getTypeConfig() : array
    {
        $query = '
            SELECT b_st.Label as type,
                   b_st.freezeThaw as freezeThaw,
                   GROUP_CONCAT(DISTINCT b_u.Label) as units,
                   GROUP_CONCAT(DISTINCT b_ct.Label) as containers,
                   GROUP_CONCAT(DISTINCT b_st2.Label) as parents
            FROM biobank_specimen_type b_st
            LEFT JOIN biobank_specimen_type_unit_rel b_st_u USING (SpecimenTypeID)
            LEFT JOIN biobank_unit b_u USING (UnitID)
            LEFT JOIN biobank_specimen_type_container_type_rel b_st_ct ON
                      b_st_ct.SpecimenTypeID=b_st.SpecimenTypeID
            LEFT JOIN biobank_container_type b_ct ON
                      b_ct.ContainerTypeID = b_st_ct.ContainerTypeID
            LEFT JOIN biobank_specimen_type_parent b_st_p ON
                      b_st_p.SpecimenTypeID = b_st.SpecimenTypeID
            LEFT JOIN biobank_specimen_type b_st2 ON
                      b_st_p.ParentSpecimenTypeID = b_st2.SpecimenTypeID 
            GROUP BY b_st.Label';
        $types    = $this->db->pselectWithIndexKey($query, [], 'type');
        foreach ($types as $label=>$type) {
            $types[$label]['units'] = DAOUtility::toArray($type['unit']);
            $types[$label]['containers'] = DAOUtility::toArray($type['containers']);
            $types[$label]['parents'] = DAOUtility::toArray($type['parents']);


            // Initialize collections for each process type
            $types[$label]['collection'] = ['protocols' => $this->getProtocolAttributes('collection')];
            $types[$label]['preparation'] = ['protocols' => $this->getProtocolAttributes('preparation')];
            $types[$label]['analysis'] = ['protocols' => $this->getProtocolAttributes('analysis')];
        }

        return $types;
    }

    /**
     * Queries all rows from the biobank_specimen_protocol_attribute_rel table
     * and returns a nested array of specimen-protocol-specific attribute data
     * with the Protocol ID as the first index, and the Attribute ID as the
     * second index.
     *
     * @return array $pA A nested array of attribute data indexed by Protocol
     *                    ID and Attribute ID
     */
    public function getProtocolAttributes(string $type) : array
    {
        $query  = "SELECT bsp.Label as protocol,
                          bsa.Label as attribute,
                          bsad.Datatype as datatype,
                          bspa.Required as required,
                          bspa.ShowInDataTable as showInDataTable
                  FROM biobank_specimen_protocol bsp
                  JOIN biobank_specimen_protocol_attribute_rel bspa
                    USING (SpecimenProtocolID)
                  JOIN biobank_specimen_attribute bsa
                    USING (SpecimenAttributeID)
                  JOIN biobank_specimen_attribute_datatype bsad
                    USING (DatatypeID)
                  JOIN biobank_specimen_type b_st
                    USING (SpecimenTypeID)
                  WHERE b_st.Label = $type";
        $protocols = $this->db->pselect($query, []);
        $pA     = []; //protocolAttributes

        foreach ($protocols as $label => $protocol) {
            $attribute =& $protocols[$label]['attribute'];

            if (!isset($pA[$label][$attribute])) {
                $pA[$label][$attribute] = [];
            }

            $attrib =& $pA[$protocol][$attribute];
            $attrib['label']           = $row['attributeLabel'];
            $attrib['datatype']      = $row['datatype'];
            $attrib['required']        = (int) $row['required'];
            $attrib['showInDataTable'] = (int) $row['showInDataTable'];
        }

        return $pA;
    }

    /**
     * This function receives a Specimen Object, converts it into a SQL format
     * and inserts it into all relevant tables in the database.
     *
     * @param Specimen $specimen The Specimen Object to be saved the
     *                           database.
     *
     * @return Specimen $specimen The Specimen Object that was saved
                                  to the database.
     */
    public function saveInstance(Specimen $specimen) : Specimen
    {
        // Converts Specimen Object into SQL format.
        $data = $this->_getSQLFromInstance($specimen);

        // Inserts or Updates the biobank_specimen table with respective data.
        $this->db->insertOnDuplicateUpdate(
            'biobank_specimen',
            $data['biobank_specimen']
        );

        $specimenId = $specimen->getId();
        if (isset($specimenId)) {
            // If update:
            // Do nothing.
        } else {
            // If insert:
            $specimen->setId((int) $this->db->getLastInsertId());
            // set the respective data array index to the value of the new
            // specimen ID
            $parentSpecimenIds = $specimen->getParentSpecimenIds();
            if (!empty($parentSpecimenIds)) {
                foreach ($parentSpecimenIds as $id) {
                    $data['biobank_specimen_parent'][$id] = [
                        'SpecimenID'       => $specimen->getId(),
                        'ParentSpecimenID' => $id,
                    ];
                }
            }
            $data['biobank_specimen_freezethaw']['SpecimenID']  = $specimen->getId();
            $data['biobank_specimen_collection']['SpecimenID']  = $specimen->getId();
            $data['biobank_specimen_preparation']['SpecimenID'] = $specimen->getId();
            $data['biobank_specimen_analysis']['SpecimenID']    = $specimen->getId();
        }

        // Insert or update biobank_specimen_collection with respective data.
        $this->db->unsafeInsertOnDuplicateUpdate(
            'biobank_specimen_collection',
            $data['biobank_specimen_collection']
        );

        // If protocol exists, insert or update biobank_specimen_preparation
        // with respective data.
        if (isset($data['biobank_specimen_preparation']['SpecimenProtocolID'])) {
            $this->db->unsafeInsertOnDuplicateUpdate(
                'biobank_specimen_preparation',
                $data['biobank_specimen_preparation']
            );
        }

        // If method exists, insert or update biobank_specimen_analysis
        // with respective data.
        if (isset($data['biobank_specimen_analysis']['SpecimenProtocolID'])) {
            $this->db->unsafeInsertOnDuplicateUpdate(
                'biobank_specimen_analysis',
                $data['biobank_specimen_analysis']
            );
        }

        // If parent exists, insert or update biobank_specimen_parent
        // with respective data.
        if (isset($data['biobank_specimen_parent'])) {
            foreach ($data['biobank_specimen_parent'] as $insert) {
                $this->db->insertOnDuplicateUpdate(
                    'biobank_specimen_parent',
                    $insert
                );
            }
        }

        // If F/T Cycle exists, insert or update biobank_specimen_freezethaw
        // with respective data.
        $fTCycle = $specimen->getFTCycle();
        if (isset($fTCycle)) {
            $this->db->insertOnDuplicateUpdate(
                'biobank_specimen_freezethaw',
                $data['biobank_specimen_freezethaw']
            );
        }

        // Prepare project data and sent it to be saved.
        foreach ($data['biobank_specimen_project_rel'] as $i => $insert) {    
            $projInsert =& $data['biobank_specimen_project_rel'][$i];
            $projInsert['SpecimenID'] = $specimen->getId();                     
        }                                                                       
        $this->_saveProject($specimen, $data);    

        return $this->getInstanceFromId($specimen->getId());
    }

    /**                                                                         
     * Saves the Specimen Project to the database.                           
     *                                                                          
     * @param Specimen $specimen 
     * @param array    $data      The data to save                             
     *                                                                          
     * @return void                                                             
     */                                                                         
    private function _saveProject(Specimen $specimen, array $data)                            
    {                                                                           
        // insert on update biobank_specimen_project_rel with relevant data.   
        $this->db->delete(                                                      
            'biobank_specimen_project_rel',                                    
            ['SpecimenID' => $specimen->getId()]                              
        );                                                                      
        foreach ($data['biobank_specimen_project_rel'] as $insert) {           
            $this->db->insert(                                                  
                'biobank_specimen_project_rel',                                
                $insert                                                         
            );                                                                  
        }                                                                       
    }      

    /**
     * This function takes a Specimen Instance and prepares the data to be
     * inserted into the database by converting it to a data array. This one to
     * one mapping is done to enable the update or insertion of data into the
     * database directly from the resulting arrays.
     *
     * @param Specimen $specimen Specimen Instance to be converted
     *
     * @return array $data Array containing the data to be inserted
     */
    private function _getSQLFromInstance(Specimen $specimen) : array
    {
        $specimenData = [
            'SpecimenID'     => $specimen->getId(),
            'ContainerID'    => $specimen->getContainerId(),
            'SpecimenTypeID' => $specimen->getTypeId(),
            'Quantity'       => $specimen->getQuantity(),
            'UnitID'         => $specimen->getUnitId(),
            'SessionID'      => $specimen->getSessionId(),
        ];

        $freezeThawData = [
            'SpecimenID'      => $specimen->getId(),
            'FreezeThawCycle' => $specimen->getFTCycle(),
        ];

        $collection     = $specimen->getCollection();
        $collectionData = [
            'SpecimenID'         => $specimen->getId(),
            'SpecimenProtocolID' => $collection->getProtocolId(),
            'Quantity'           => $collection->getQuantity(),
            'UnitID'             => $collection->getUnitId(),
            'CenterID'           => $collection->getCenterId(),
            'ExaminerID'         => $collection->getExaminerId(),
            'Date'               => $collection->getDate()->format('Y-m-d H:i'),
            'Time'               => $collection->getTime()->format('Y-m-d H:i'),
            'Comments'           => $collection->getComments(),
        ];
        $collectionData['Data'] = !empty($collection->getData()) ?
            json_encode($collection->getData()) : null;

        $preparation     = $specimen->getPreparation();
        $preparationData = [];
        if (isset($preparation)) {
            $preparationData = [
                'SpecimenID'         => $specimen->getId(),
                'SpecimenProtocolID' => $preparation->getProtocolId(),
                'CenterID'           => $preparation->getCenterId(),
                'ExaminerID'         => $preparation->getExaminerId(),
                'Date'               => $preparation->getDate()->format('Y-m-d H:i'),
                'Time'               => $preparation->getTime()->format('Y-m-d H:i'),
                'Comments'           => $preparation->getComments(),
            ];

            $preparationData['Data'] = !empty($preparation->getData()) ?
                json_encode($preparation->getData()) : null;
        }

        $analysis     = $specimen->getAnalysis();
        $analysisData = [];
        if (isset($analysis)) {
            $analysisData = [
                'SpecimenID'         => $specimen->getId(),
                'SpecimenProtocolID' => $analysis->getProtocolId(),
                'CenterID'           => $analysis->getCenterId(),
                'ExaminerID'         => $analysis->getExaminerId(),
                'Date'               => $analysis->getDate()->format('Y-m-d H:i'),
                'Time'               => $analysis->getTime()->format('Y-m-d H:i'),
                'Comments'           => $analysis->getComments(),
            ];

            if ($analysis->getData()) {
                $analysisData['Data'] = json_encode($analysis->getData());
            }
            $analysisData['Data'] = !empty($analysis->getData()) ?
                json_encode($analysis->getData()) : null;
        }

        $specimenProjectData = [];                                            
        foreach ($specimen->getProjectIds() as $id) {                         
            $specimenProjectData[] = ['ProjectID' => $id];
        }

        return [
            'biobank_specimen'              => $specimenData,
            'biobank_specimen_freezethaw'   => $freezeThawData,
            'biobank_specimen_collection'   => $collectionData,
            'biobank_specimen_preparation'  => $preparationData,
            'biobank_specimen_analysis'     => $analysisData,
            'biobank_specimen_project_rel'  => $specimenProjectData,
        ];
    }

    /**
     * This function takes an array that resulted from an SQL query and
     * instantiates it as a Specimen Object.
     *
     * @param array $data              Values to be converted to array.
     *
     * @return Specimen
     */
    private function _getInstanceFromSQL(array $data) : Specimen {
        $specimen = new Specimen($this->loris);
        $specimen->setBarcode((string) $data['Barcode']);
        $specimen->setType((string) $data['SpecimenType']);
        $specimen->setQuantity((string) $data['Quantity']);
        $specimen->setUnit((string) $data['Unit']);

        if (isset($data['CenterID'])) {
            $specimen->setCenterId(new \CenterID($data['CenterID']));
        }
        if (isset($data['Center'])) {
            $specimen->setCenter($data['Center']);
        }
        if (isset($data['ProjectIDs'])) {
            $projectIds = array_map(fn($id) => new \ProjectID($id),
                DAOUtility::toArray($data['ProjectIDs']));
            $specimen->setProjectIds($projectIds);
        }
        if (isset($data['Projects'])) {
            $projects = DAOUtility::toArray($data['Projects']);
            $specimen->setProjects($projects);
        }
        if (isset($data['FreezeThawCycle'])) {
            $specimen->setFTCycle((int) $data['FreezeThawCycle']);
        }
        if (!empty($data['Parents'])) {
            $parents = DAOUtility::toArray($data['Parents']);
            $specimen->setParents($parents);
        } else {
            $specimen->setParents([]);
        }
        if (isset($data['Candidate'])) {
            $specimen->setCandidate($data['Candidate']);
        }
        if (isset($data['Session'])) {
            $specimen->setSession($data['Session']);
        }
        if (isset($data['Pool'])) {
            $specimen->setPool($data['Pool']);
        }

        $collection = new Collection();
        if (isset($data['CollectionProtocol'])) {
            $collection->setProtocol($data['CollectionProtocol']);
        }
        if (isset($data['CollectionQuantity'])) {
            $collection->setQuantity($data['CollectionQuantity']);
        }
        if (isset($data['CollectionUnit'])) {
            $collection->setUnit($data['CollectionUnit']);
        }
        if (isset($data['CollectionCenter'])) {
            $collection->setCenter($data['CollectionCenter']);
        }
        if (isset($data['CollectionExaminer'])) {
            $collection->setExaminer($data['CollectionExaminer']);
        }
        if (isset($data['CollectionDate'])) {
            $collection->setDate(new \DateTime($data['CollectionDate']));
        }
        if (isset($data['CollectionTime'])) {
            $collection->setTime(
                new \DateTime($data['CollectionTime'])
            );
        }
        if (isset($data['CollectionComments'])) {
            $collection->setComments($data['CollectionComments']);
        }
        if (isset($data['CollectionData'])) {
            $collection->setData(
                json_decode($data['CollectionData'], true)
            );
        }
        $specimen->setCollection($collection);

        // XXX: These checks for instantiating $prepartion and $analysis are not
        // the best. Perhaps they should be queried separately and it should be
        // checked whether or not the array itself exists.
        $preparation = new Preparation();
        if ($data['PreparationProtocol']) {
            if (isset($data['PreparationProtocol'])) {
                $preparation->setProtocol(
                    (string) $data['PreparationProtocol']
                );
            }
            if (isset($data['PreparationCenter'])) {
                $preparation->setCenter(
                    (string) $data['PreparationCenter']
                );
            }
            if (isset($data['PreparationExaminer'])) {
                $preparation->setExaminer(
                    (string) $data['PreparationExaminer']
                );
            }
            if (isset($data['PreparationDate'])) {
                $preparation->setDate(
                    new \DateTime($data['PreparationDate'])
                );
            }
            if (isset($data['PreparationTime'])) {
                $preparation->setTime(
                    new \DateTime($data['PreparationTime'])
                );
            }
            if (isset($data['PreparationComments'])) {
                $preparation->setComments(
                    (string) $data['PreparationComments']
                );
            }
            if (isset($data['PreparationData'])) {
                $preparation->setData(
                    json_decode($data['PreparationData'], true)
                );
            }
            $specimen->setPreparation($preparation);
        }

        if ($data['AnalysisProtocol']) {
            $analysis = new Analysis();
            if (isset($data['AnalysisProtocol'])) {
                $analysis->setProtocol((string) $data['AnalysisProtocol']);
            }
            if (isset($data['AnalysisCenter'])) {
                $analysis->setCenter((string) $data['AnalysisCenter']);
            }
            if (isset($data['AnalysisExaminer'])) {
                $analysis->setExaminer((string) $data['AnalysisExaminer']);
            }
            if (isset($data['AnalysisDate'])) {
                $analysis->setDate(new \DateTime($data['AnalysisDate']));
            }
            if (isset($data['AnalysisTime'])) {
                $analysis->setTime(new \DateTime($data['AnalysisTime']));
            }
            if (isset($data['AnalysisComments'])) {
                $analysis->setComments((string) $data['AnalysisComments']);
            }
            if (isset($data['AnalysisData'])) {
                $analysis->setData(json_decode($data['AnalysisData'], true));
            }
            $specimen->setAnalysis($analysis);
        }

        return $specimen;
    }
}
