<?php declare(strict_types=1);
/**
 * Pool Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Pool Object instances.
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
 * Pool Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Pool Object instances.
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
class Pool implements
    \JsonSerializable,
    \LORIS\Data\DataInstance
{
    private string $label;
    private float $quantity;
    private string $unit;
    private array $specimenBarcodes;
    private string $candidate;
    private string $session;
    private string $type;
    private string $center;
    private array $projects;
    private \DateTime $date;
    private \DateTime $time;

    public function __construct(array $data) {
        $this->setLabel($data['label']);
        $this->setQuantity((float) $data['quantity']);
        $this->setUnit($data['unit']);
        $this->setSpecimens($data['specimens']);
        $this->setCandidate($data['candidate']);
        $this->setSession($data['session']);
        $this->setType($data['type']);
        $this->setCenter($data['center']);
        $this->setProjects($data['projects']);
        $this->setDate(new \DateTime($data['date']));
        $this->setTime(new \DateTime($data['time']));
    }

    public function getLabel(): string {
        return $this->label;
    }

    public function getQuantity(): float {
        return $this->quantity;
    }

    public function getUnit(): string {
        return $this->unit;
    }

    public function getSpecimenBarcodes(): array {
        return $this->specimenBarcodes;
    }

    public function getCandidate(): string {
        return $this->candidate;
    }

    public function getSession(): string {
        return $this->session;
    }

    public function getType(): string {
        return $this->type;
    }

    public function getCenter(): string {
        return $this->center;
    }

    public function getProjects(): array {
        return $this->projects;
    }

    public function getDate(): \DateTime {
        return $this->date;
    }

    public function getTime(): \DateTime {
        return $this->time;
    }

    public function setLabel(string $label): void {
        if (empty($label)) {
            throw new InvalidArgumentException("Label cannot be empty.");
        }
        $this->label = $label;
    }

    public function setQuantity(float $quantity): void {
        if ($quantity < 0) {
            throw new InvalidArgumentException("Quantity cannot be negative.");
        }
        $this->quantity = $quantity;
    }

    public function setUnit(string $unit): void {
        if (empty($unit)) {
            throw new InvalidArgumentException("Unit cannot be empty.");
        }
        $this->unit = $unit;
    }

    public function setSpecimens(array $specimens): void {
        $this->specimens = $specimens;
    }

    public function setCandidate(string $candidate): void {
        if (empty($candidate)) {
            throw new InvalidArgumentException("Candidate cannot be empty.");
        }
        $this->candidate = $candidate;
    }

    public function setSession(string $session): void {
        if (empty($session)) {
            throw new InvalidArgumentException("Session cannot be empty.");
        }
        $this->session = $session;
    }

    public function setType(string $type): void {
        if (empty($type)) {
            throw new InvalidArgumentException("Type cannot be empty.");
        }
        $this->type = $type;
    }

    public function setCenter(string $center): void {
        if (empty($center)) {
            throw new InvalidArgumentException("Center cannot be empty.");
        }
        $this->center = $center;
    }

    public function setProjects(array $projects): void {
        $this->projects = $projects;
    }

    public function setDate(\DateTime $date): void {
        $this->date = $date;
    }

    public function setTime(\DateTime $time): void {
        $this->time = $time;
    }

    /**
     * Loads a Pool Object from an array.
     *
     * @param array $data Data to be used to load the Pool Object.
     *
     * @return Pool
     */
    public function fromArray(array $data) : Pool
    {
        if (isset($data['label'])) {
            $this->setLabel((string) $data['label']);
        }
        if (isset($data['quantity'])) {
            $this->setQuantity((float) $data['quantity']);
        }
        if (isset($data['unit'])) {
            $this->setUnit($data['unit']);
        }
        if (isset($data['specimens'])) {
            $this->setSpecimens($data['specimens']);
        }
        if (isset($data['specimenBarcodes'])) {
            $this->setSpecimenBarcodes($data['specimenBarcodes']);
        }
        if (isset($data['candidate'])) {
            $this->setCandidate((string) $data['candidate']);
        }
        if (isset($data['session'])) {
            $this->setSession($data['session']);
        }
        if (isset($data['type'])) {
            $this->setType($data['type']);
        }
        if (isset($data['center'])) {
            $this->setCenter($data['center']);
        }
        if (isset($data['projects'])) {
            $this->setProjects($data['projects']);
        }
        if (isset($data['date'])) {
            $this->setDate(new \DateTime($data['date']));
        }
        if (isset($data['time'])) {
            $this->setTime(new \DateTime($data['time']));
        }

        return $this;
    }

    /**
     * A wrapper function for the jsonSerialize method of the Pool Instance
     * Class. This converts the Pool Instance into an array.
     *
     * @return array Pool Array.
     */
    public function toArray() : array
    {
        return $this->jsonSerialize();
    }

    /**
     * Returns an array representing the state of the Pool.
     *
     * @return array
     */
    public function jsonSerialize() : array
    {
        // TODO: Date and Time formats should be gotten from some sort of
        // config setting.
        return [
            'label'            => $this->label,
            'quantity'         => $this->quantity,
            'unit'             => $this->unit,
            'specimens'        => $this->specimens,
            'candidate'        => $this->candidate,
            'session'          => $this->session,
            'type'             => $this->type,
            'center'           => $this->center,
            'projects'         => $this->projects,
            'date'             => $this->date->format('d-m-Y'),
            'time'             => $this->time->format('H:i'),
        ];
    }

    /**
     * Returns a JSON encoded string of the state of the Pool
     *
     * @return string JSON encoded string of Pool data
     */
    public function toJSON() : string
    {
        return json_encode($this);
    }
}

