<?php declare(strict_types=1);
/**
 * Container Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Container Object instances.
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
 * Container Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Container Object instances.
 *
 * PHP Version 7.2
 *
 * @category Loris
 * @package  Biobank
 * @author   Henri Rabalais <hrabalais.mcin@gmail.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://www.github.com/aces/Loris/
 */

class Container extends DTO implements
    \JsonSerializable,
    \LORIS\Data\DataInstance
{
    /**
     * Persistent Instance variables.
     *
     * These properties describe a container entity.
     *
     * @var int          $id
     * @var string       $barcode
     * @var int          $type
     * @var int          $dimension
     * @var float        $temperature
     * @var int          $status
     * @var array        $shipments
     * @var \CenterID    $center
     * @var ?string      $parent
     * @var array        $children
     * @var int          $coordinate
     * @var string       $lotNumber
     * @var \DateTime    $expirationDate
     * @var string       $comments
     *
     * @access private
     */

    private $id;
    private $barcode;
    private $typeId;
    private $type;
    private $dimension;
    private $temperature;
    private $status;
    private $shipments;
    private $centerId;
    private $center;
    private ?string $parent = null;
    private $children;
    private $coordinate;
    private $lotNumber;
    private $expirationDate;
    private $comments;

    /**
     * Sets the container's barcode
     *
     * @param string $barcode the container's barcode
     *
     * @return void
     */
    public function setBarcode(string $barcode) : void
    {
        $this->barcode = $barcode;
    }

    /**
     * Gets the container's barcode
     *
     * @return ?string
     */
    public function getBarcode() : ?string
    {
        return $this->barcode;
    }

    /**
     * Sets the ID of the container type
     *
     * @param string $type container type
     *
     * @return void
     */
    public function setType(string $type) : void
    {
        $this->type = $type;
    }

    /**
     * Gets the container type
     *
     * @return ?string
     */
    public function getType() : ?string
    {
        return $this->type;
    }

    /**
     * Sets the ID of the container's dimensions
     *
     * @param Dimension $dimension ID of the container's dimensions
     *
     * @return void
     */
    public function setDimension($dimension) : void
    {
        $this->dimension = $dimension;
    }

    /**
     * Gets the ID of the container's dimensions
     *
     * @return ?Dimension
     */
    public function getDimension()
    {
        return $this->dimension;
    }

    /**
     * Sets the container's temperature in Celsius.
     *
     * @param float $temperature the container's temperature.
     *
     * @return void
     */
    public function setTemperature(float $temperature) : void
    {
        $this->temperature = $temperature;
    }

    /**
     * Gets the container's temperature in Celsius.
     *
     * @return ?float
     */
    public function getTemperature() : ?float
    {
        return $this->temperature;
    }

    /**
     * Sets the container's shipments
     *
     * @param array $shipment the container's shipments
     *
     * @return void
     */
    public function setShipments(array $shipments) : void
    {
        $this->shipments = $shipments;
    }

    /**
     * Gets the the container's shipments
     *
     * @return ?array
     */
    public function getShipments() : ?array
    {
        return $this->shipments;
    }

    /**
     * Sets the the container's current status
     *
     * @paramstring $status container's current status
     *
     * @return void
     */
    public function setStatus(string $status) : void
    {
        $this->status = $status;
    }

    /**
     * Gets the container's current status
     *
     * @return ?string
     */
    public function getStatus() : ?string
    {
        return $this->status;
    }

    /**
     * Sets the container's current center
     *
     * @param string $center the container's current center
     *
     * @return void
     */
    public function setCenter(string $center) : void
    {
        $this->center = $center;
    }

    /**
     * Gets the container's current center
     *
     * @return string
     */
    public function getCenter() : string
    {
        return $this->center;
    }

    /**
     * Sets the parent container
     *
     * @param ?string $parentContainer the parent container
     *
     * @return void
     */
    public function setParent(?string $parent) : void
    {
        $this->parent = $parent;
    }

    /**
     * Gets the parent container's barcode
     *
     * @return ?string
     */
    public function getParent() : ?string
    {
        return $this->parent;
    }

    /**
     * Sets the the children containers
     *
     * @param array $children array of IDs of children container
     *
     * @return void
     */
    public function setChildren(array $children) : void
    {
        $this->children = $children;
    }

    /**
     * Gets the the children containers
     *
     * @return ?array
     */
    public function getChildren() : ?array
    {
        return $this->children;
    }

    /**
     * Sets the container's current coordinate in storage
     *
     * @param ?int $coordinate the container's current coordinate
     *
     * @return void
     */
    public function setCoordinate(?int $coordinate = null) : void
    {
        $this->coordinate = intval($coordinate);
    }

    /**
     * Gets the container's current coordinate in storage
     *
     * @return ?int
     */
    public function getCoordinate() : ?int
    {
        return $this->coordinate;
    }

    /**
     * Sets the container's lot number
     *
     * @param ?string $lotNumber the container's lot number.
     *
     * @return void
     */
    public function setLotNumber(?string $lotNumber = null) : void
    {
        $this->lotNumber = $lotNumber;
    }

    /**
     * Gets the container's lot number in storage
     *
     * @return ?string
     */
    public function getLotNumber() : ?string
    {
        return $this->lotNumber;
    }

    /**
     * Sets the expiration date
     *
     * @param ?\DateTime $date the expiration date
     *
     * @return void
     */
    public function setExpirationDate(?string $date = null) : void
    {
        if (is_string($date)) {
            $this->expirationDate = new \DateTime($date);
        } elseif ($date instanceof \DateTime) {
            $this->expirationDate = $date;
        } else {
            // Handle null or invalid date format
            $this->expirationDate = null;
        }
        $this->setExpirationDate(new \DateTime($date));
    }

    /**
     * Gets the expiration date
     *
     * @return ?\DateTime
     */
    public function getExpirationDate() : ?\DateTime
    {
        return $this->expirationDate;
    }

    /**
     * Sets the container's status comments.
     *
     * @param string $comments the container's status comments
     *
     * @return void
     */
    public function setComments(string $comments) : void
    {
        $this->comments = $comments;
    }

    /**
     * Gets the container's status comments.
     *
     * @return ?string
     */
    public function getComments() : ?string
    {
        return $this->comments;
    }
}
