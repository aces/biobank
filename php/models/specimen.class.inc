<?php declare(strict_types=1);
/**
 * Specimen Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Specimen Object instances.
 *
 * PHP Version 7.2
 *
 * @category   Entity
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
namespace LORIS\biobank;

/**
 * Specimen Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Specimen Object instances.
 *
 * PHP Version 7.2
 *
 * @category   Entity
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class Specimen implements
    \JsonSerializable,
    \LORIS\Data\DataInstance
{
    /**
     * Persistent Instance variables.
     *
     * These properties describe a specimen entity.
     *
     * @var Container   $container
     * @var string      $barcode
     * @var string      $type
     * @var string      $quantity
     * @var string      $unit
     * @var string      $center
     * @var array       $Projects
     * @var int         $fTCycle
     * @var array       $parents
     * @var string      $candidate
     * @var string      $session
     * @var string      $pool
     * @var Collection  $collection
     * @var Preparation $preparation
     * @var Analysis    $analysis
     */
    private Container $container;
    private string $barcode;
    private $type;
    private $quantity;
    private $unit;
    private $fTCycle;
    private $center;
    private $projects;
    private $parents;
    private $candidate;
    private $session;
    private $pool;
    private $collection;
    private $preparation;
    private $analysis;

    /**
     * set the specimen's container
     *
     * @param \Container $container
     *
     * @return void
     */
    public function setContainer($container) : void
    {
        $this->container = $container;
    }

    /**
     * Gets the specimen's container
     *
     * @return ?\Container
     */
    public function getContainer() : ?\Container
    {
        return $this->container;
    }

    /**
     * Sets the specimen's barcode
     *
     * @param string $barcode
     *
     * @return void
     */
    public function setBarcode($barcode) : void
    {
        $this->barcode = $barcode;
    }

    /**
     * Gets the specimen's barcode
     *
     * @return string
     */
    public function getBarcode() : string
    {
        return $this->barcode;
    }

    /**
     * Sets the specimen type
     *
     * @param string $type the specimen type
     *
     * @return void
     */
    public function setType(string $type) : void
    {
        $this->type = $type;
    }

    /**
     * Gets the specimen type
     *
     * @return string
     */
    public function getType() : string
    {
        return $this->type;
    }

    /**
     * Sets the current amount of specimen available
     *
     * @param string $quantity the current amount of specimen available
     *
     * @return void
     */
    public function setQuantity(string $quantity) : void
    {
        $this->quantity = $quantity;
    }

    /**
     * Gets the current amount of specimen available
     *
     * @return ?string
     */
    public function getQuantity() : ?string
    {
        return $this->quantity;
    }

    /**
     * Sets the specimen's unit of quantity
     *
     * @param string $unit the quantity's unit
     *
     * @return void
     */
    public function setUnit(string $unit) : void
    {
        $this->unit = $unit;
    }

    /**
     * Gets the specimen's unit of quantity
     *
     * @return ?string
     */
    public function getUnit() : ?string
    {
        return $this->unit;
    }

    /**
     * Sets the specimen's current Freeze-Thaw cycle.
     *
     * @param int $fTCycle the specimen's current F-T cycle
     *
     * @return void
     */
    public function setFTCycle(int $fTCycle) : void
    {
        if ($fTCycle >= 0) {
            $this->fTCycle = $fTCycle;
        }
    }

    /**
     * Gets the specimen's current Freeze-Thaw cycle
     *
     * @return ?int
     */
    public function getFTCycle() : ?int
    {
        return $this->fTCycle;
    }

    /**
     * Sets the parent specimen's.
     *
     * @param array $parents the parent specimens
     *
     * @return void
     */
    public function setParents(array $parents) : void
    {
        $this->parents = $parents;
    }

    /**
     * Gets the parent specimens
     *
     * @return ?array
     */
    public function getParents() : ?array
    {
        return $this->parents;
    }

    /**
     * Sets the candidate to which the specimen belongs
     *
     * @param string $candidate the specimen's donor
     *
     * @return void
     */
    public function setCandidate(string $candidate) : void
    {
        $this->candidate = $candidate;
    }

    /**
     * Gets the candidate to which the specimen belongs
     *
     * @return ?string
     */
    public function getCandidate() : ?string
    {
        return $this->candidate;
    }

    /**
     * Gets the candidate's session/timepoint in which the specimen
     * was collected.
     *
     * @return ?string
     */
    public function setSession(string $session) : void
    {
        $this->session = $session;
    }


    /**
     * Gets the candidate's session/timepoint in which the specimen
     * was collected.
     *
     * @return ?string
     */
    public function getSession() : ?string
    {
        return $this->session;
    }

    /**
     * Sets the specimen's pool group, if it exists.
     *
     * @param string $pool the specimen's pool group
     *
     * @return void
     */
    public function setPool(string $pool) : void
    {
        $this->pool = $pool;
    }

    /**
     * Gets the specimen's pool group.
     *
     * @return ?string
     */
    public function getPool() : ?string
    {
        return $this->pool;
    }

    /**
     * Sets a Collection object for the Specimen.
     *
     * @param Collection $collection A collection instance
     *
     * @return void
     */
    public function setCollection(Collection $collection) : void
    {
        $this->collection = $collection;
    }

    /**
     * Gets an associative array of data stored for the specimen's collection stage
     *
     * @return ?Collection
     */
    public function getCollection() : ?Collection
    {
        return $this->collection;
    }

    /**
     * Sets a Preparation object for the Specimen.
     *
     * @param Preparation $preparation A Preparation Instance
     *
     * @return void
     */
    public function setPreparation(Preparation $preparation) : void
    {
        $this->preparation = $preparation;
    }

    /**
     * Gets an associative array of data stored for the specimen's preparation
     * stage.
     *
     * @return Preparation|null
     */
    public function getPreparation() : ?Preparation
    {
        return $this->preparation;
    }

    /**
     * Sets an Analysis object for the Specimen.
     *
     * @param Analysis $analysis An Analysis Instance.
     *
     * @return void
     */
    public function setAnalysis(Analysis $analysis) : void
    {
        $this->analysis = $analysis;
    }

    /**
     * Gets an associative array of data stored for the specimen's analysis stage
     *
     * @return Analysis|null
     */
    public function getAnalysis() : ?Analysis
    {
        return $this->analysis;
    }

    public function setCenter(string $center)
    {
        $this->center = $center;
    }

    /**
     * Return Center of affiliated specimen container.
     *
     * @return string
     */
    public function getCenter() : string
    {
        return $this->center;
    }

    public function setProjects(array $projects)
    {
        $this->projects = $projects;
    }

    /**
     * Return Project of affiliated specimen container.
     *
     * @return array
     */
    public function getProjects() : array
    {
        return $this->projects;
    }

    /**
     * Instatiates a Specimen Object using the values sent from the front-end.
     *
     * @param array $data Data to be used to instantiate the Specimen Object.
     *
     * @return Specimen $specimen Instantiated Specimen Object
     */
    public function fromArray(array $data) : Specimen
    {
        if (isset($data['container'])) {
            $this->setContainer((string) $data['container']);
        }
        if (isset($data['barcode'])) {
            $this->setBarcode((string) $data['barcode']);
        }
        if (isset($data['type'])) {
            $this->setType((string) $data['type']);
        }
        if (isset($data['quantity'])) {
            $this->setQuantity((string) $data['quantity']);
        }
        if (isset($data['unit'])) {
            $this->setUnit($data['unit']);
        }
        if (isset($data['center'])) {
            $this->setCenter($data['center']);
        }
        if (isset($data['projects'])) {
            $this->setProjects($data['projects']);
        }
        if (isset($data['fTCycle'])) {
            $this->setFTCycle((int) $data['fTCycle']);
        }
        if (isset($data['parents'])) {
            $this->setParents($data['parents']);
        }
        if (isset($data['candidate'])) {
            $this->setCandidate((string) $data['candidate']);
        }
        if (isset($data['session'])) {
            $this->setSession((string) $data['session']);
        }
        if (isset($data['pool'])) {
            $this->setPool($data['pool']);
        }

        if (isset($data['collection'])) {
            $collection = (new Collection())->fromArray($data['collection']);
            assert($collection instanceof Collection);
            $this->setCollection($collection);
        }

        if (isset($data['preparation'])) {
            $preparation = (new Preparation())->fromArray($data['preparation']);
            assert($preparation instanceof Preparation);
            $this->setPreparation($preparation);
        }
        if (isset($data['analysis'])) {
            $analysis = (new Analysis())->fromArray($data['analysis']);
            assert($analysis instanceof Analysis);
            $this->setAnalysis($analysis);
        }

        return $this;
    }

    /**
     * A wrapper function for the jsonSerialize method of the Specimen Instance
     * Class. This converts this Specimen Instance into an array
     *
     * @return array
     */
    public function toArray() : array
    {
        return $this->jsonSerialize();
    }

    /**
     * Returns an array representing the state of the Specimen.
     *
     * @return array
     */
    public function jsonSerialize() : array
    {
        $array = [
            'barcode'    => $this->barcode,
            'container'  => $this->container,
            'type'       => $this->type,
            'quantity'   => $this->quantity,
            'unit'       => $this->unit,
            'center'     => $this->center,
            'projects'   => $this->projects,
            'fTCycle'    => $this->fTCycle,
            'parents'    => $this->parents,
            'candidate'  => $this->candidate,
            'session'    => $this->session,
            'pool'       => $this->pool,
            'collection' => $this->collection->jsonSerialize(),
        ];
        if ($this->preparation) {
            $array['preparation'] = $this->preparation->jsonSerialize();
        }
        if ($this->analysis) {
            $array['analysis'] = $this->analysis->jsonSerialize();
        }
        return $array;
    }

    /**
     * Returns a JSON encoded string of the state of the Specimen
     *
     * @return string JSON encoded string of Specimen data
     */
    public function toJSON() : string
    {
        return json_encode($this);
    }
}
