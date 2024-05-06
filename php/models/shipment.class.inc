<?php declare(strict_types=1);
/**
 * Shipment Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Shipment Object instances.
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
 * Shipment Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Shipment Object instances.
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
class Shipment implements
    \JsonSerializable,
    \LORIS\Data\DataInstance,
{
    /**
     * Persistent Instance variables.
     *
     * These properties describe a shipment entity.
     *
     * @var string     $barcode
     * @var string     $type
     * @var \CenterID  $destinationCenterId
     * @var array      $containerIds
     * @var array      $containerBarcodes
     * @var array      $logs
     */
    public ?string  $barcode;
    public ?string  $type;
    public ?string  $destinationCenter;
    public ?array   $containers;
    public ?array   $logs;

    /**
     * Initiliazes a new instance of the Shipment Class
     *
     * @param array $data The data for the shipment in array format.
     */
    function __construct(array $data)
    {
        $this->barcode = $data['barcode'] ?? null;
        $this->type    = $data['type'] ?? null;
        $this->destinationCenter = $data['destinationCenter'] ?? null;
        $this->containers   = $data['containers'] ?? null;
        $this->_setLogs($data['logs']) ?? null;
    }

    /**
     * Gets the tracking number of the shipment
     *
     * @return ?string
     */
    public function getBarcode() : ?string
    {
        return $this->barcode;
    }

    /**
     * Gets the type of the shipment
     *
     * @return ?string
     */
    public function getType() : ?string
    {
        return $this->type;
    }

    /**
     * Gets the origin center of the shipment
     *
     * @return \CenterID
     */
    public function getOriginCenter() : string
    {
        if (!empty($this->getLogs())) {
            return $this->getLogs()[0]->center;
        }
    }

    /**
     * Gets the destination center of the shipment
     *
     * @return \CenterID
     */
    public function getDestinationCenter() : string
    {
        return $this->destinationCenter;
    }

    /**
     * Gets the status of the shipment
     *
     * @return ?string
     */
    public function getStatus() : ?string
    {
        if (!empty($this->getLogs())) {
            $latestLog = $this->getLogs()[array_key_last($this->getLogs())];
            return $latestLog->status;
        }
    }

    /**
     * Returns true if the shipment is not received.
     *
     * @return bool
     */
    public function getActive() : bool
    {
        return $this->getStatus() !== 'received';
    }

    /**
     * Gets the containers of the shipment
     *
     * @return ?array
     */
    public function getContainers() : ?array
    {
        return $this->containers;
    }

    /**
     * Sets the logs for the shipment
     *
     * @param array $logs the shipment's logs
     *
     * @return void
     */
    private function _setLogs(array $logs) : void
    {
        foreach ($logs as $i => $log) {
            $this->logs[$i] = new Log($log);
        }
    }

    /**
     * Gets the logs of the shipment
     *
     * @return array
     */
    public function getLogs() : array
    {
        return $this->logs;
    }

    /**
     * Return Center ID of affiliated specimen container.
     *
     * @return array
     */
    public function getCenterIds() : array
    {
       switch ($this->getStatus()) {
       // TODO: Instead of checking for strings, this ideally would check for
       // boolean values from functions such as getActive(). This way, the
       // strings will be relegated to singular functions that then serve
       // functional purposes.
       case 'received':
           return [$this->getDestinationCenter()];
       case 'shipped':
           return [$this->getOriginCenter(), $this->getDestinationCenter];
       default:
           return [$this->getOriginCenter()];
       }
    }

    /**
     * A wrapper function for the jsonSerialize method of the Shipment Instance
     * Class. This converts the Shipment Instance into an array.
     *
     * @return array Shipment Array.
     */
    public function toArray() : array
    {
        return $this->jsonSerialize();
    }

    /**
     * Returns an array representing the state of the Shipment
     *
     * @return array
     */
    public function jsonSerialize() : array
    {
        return [
            'barcode'           => $this->barcode,
            'type'              => $this->type,
            'originCenter'      => $this->getOriginCenter(),
            'destinationCenter' => $this->destinationCenter,
            'containers'        => $this->containerBarcodes,
            'status'            => $this->getStatus(),
            'active'            => $this->getActive(),
            'logs'              => $this->logs,
        ];
    }

    /**
     * Returns a JSON encoded string of the state of the Shipment
     *
     * @return string JSON encoded string of Shipment data
     */
    public function toJSON() : string
    {
        return json_encode($this);
    }
}
