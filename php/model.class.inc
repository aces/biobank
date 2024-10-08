<?php declare(strict_types=1);

namespace LORIS\biobank;

use LORIS\biobank\data;
use ReflectionClass;
use ReflectionProperty;

/**
 * Abstract class Model
 *
 * Represents a generic model within the Biobank module. This class
 * implements basic functionality such as extracting data, converting
 * to arrays, and handling JSON serialization.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class Model implements \LORIS\Data\DataInstance
{
    /**
     * Holds an array of properties that have been set in the model.
     *
     * @var array
     */
    protected array $setProperties = [];

    /**
     * Get the associated schema for the model.
     *
     * @return SchemaInterface The schema associated with this model.
     */    
    public function getSchema(): Schema
    {
    }

    /**
     * Converts the current object instance into an array, including the ID.
     *
     * @return array The object instance as an associative array.
     */    
    public function toArray(): array
    {
        return array_merge(['id' => $this->id], $this->extract());
    }

    /**
     * Serializes the object to a JSON-serializable array.
     *
     * @return array The object instance as a JSON-serializable array.
     */    
    public function jsonSerialize(): array {
        return array_merge($this->extract(), $this->toJSON());
    }

    /**
     * Returns additional properties to be included in JSON serialization.
     *
     * Child classes should override this method to add extra properties for
     * JSON serialization.
     *
     * @return array Additional properties for JSON serialization.
     */    
    protected function toJSON(): array {
        return [];
    }

    /**
     * Extracts properties from the current object instance into an array.
     *
     * This method will include only public properties and can optionally
     * include the object's ID.
     *
     * @param bool $includeId Whether to include the ID in the extracted array.
     * @return array          The extracted properties as an associative array.
     */    
    protected function extract(): array
    {
        $data = [];
        $reflect = new ReflectionClass($this);
        $props = $reflect->getProperties(ReflectionProperty::IS_PUBLIC);
    
        foreach ($props as $prop) {
            $key = $prop->getName();
            $value = $prop->getValue($this);

            if (!is_null($value)) {
                $data[$key] = $value;
            }
        }
    
        return $data;
    }
}
