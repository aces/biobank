<?php declare(strict_types=1);
/**
 * Collection Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Collection Object instances.
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
 * Collection Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Collection Object instances.
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
class Collection extends Process
{
    /**
     * Persistent Instance variables.
     *
     * These variables describe a collection instance.
     *
     * @var string $quantity
     * @var string $unit
     */
    private $quantity;
    private $unit;

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
    public function setUnit(string $unit) : void
    {
        $this->unit = $unit;
    }

    /**
     * Gets the specimen's unit of quantity
     *
     * @return ?int
     */
    public function getUnit() : ?string
    {
        return $this->unit;
    }

    /**
     * Instatiates a Collection Object using the values sent from the client-side.
     *
     * @param array $data Data to be used to instantiate the Collection Object.
     *
     * @return Process
     */
    public function fromArray(array $data) : Process
    {
        parent::fromArray($data);
        isset($data['quantity']) && $this->setQuantity((string) $data['quantity']);
        isset($data['unit'])   && $this->setUnit((string) $data['unit']);

        return $this;
    }

    /**
     * A wrapper function for the jsonSerialize method of the Collection Instance
     * Class. This converts this Specimen Collection into an array
     *
     * @return array Collection Array
     */
    public function toArray()
    {
        return $this->jsonSerialize();
    }

    /**
     * Returns an array representing the state of the Collection.
     *
     * @return array
     */
    public function jsonSerialize() : array
    {
        $parent = parent::jsonSerialize();
        return array_merge(
            $parent,
            [
                'quantity' => $this->quantity,
                'unit'     => $this->unit,
            ]
        );
    }
}

