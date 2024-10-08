<?php declare(strict_types=1);

namespace LORIS\biobank;

use ArrayObject;
use InvalidArgumentException;

/**
 * Abstract class Models
 *
 * Represents a collection of model instances, extending ArrayObject to
 * provide custom type validation, JSON serialization, and management
 * of model data.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class Models extends ArrayObject implements \JsonSerializable
{
    /**
     * The name of the model class to validate against.
     *
     * Note: This is a static property, which means all instances of 
     * extending classes will share the same model type. This could 
     * lead to issues with inheritance if different model types are 
     * expected across subclasses.
     *
     * @var string
     */    
    public static string $model;

    /**
     * Constructs the Models collection.
     *
     * Initializes an empty ArrayObject and populates it with the given 
     * items, validating that each item is of the correct model type.
     *
     * @param array $items The initial items to populate the collection.
     */    
    public function __construct(array $items = [])
    {
        parent::__construct([], ArrayObject::ARRAY_AS_PROPS);

        foreach ($items as $item) {
            $this->validateType($item);
            $this[] = $item;
        }
    }

    /**
     * Sets a value at the specified offset, with type validation.
     *
     * @param mixed $key   The key at which to set the value.
     * @param mixed $value The value to set, validated against the model type.
     * @throws InvalidArgumentException If the value is not an instance of
     *                                  the expected model.
     */    
    public function offsetSet($key, $value): void
    {
        $this->validateType($value);
        parent::offsetSet($key, $value);
    }

    /**
     * Appends a value to the collection, with type validation.
     *
     * @param mixed $value The value to append, validated against the model
     *                     type.
     * @throws InvalidArgumentException If the value is not an instance of
     *                                  the expected model.
     */    
    public function append($value): void
    {
        $this->validateType($value);
        parent::append($value);
    }

    /**
     * Retrieves an array of IDs from the model instances.
     *
     * Casts the collection to an array and applies a callback to extract 
     * the `id` property from each model.
     *
     * @return array An array of model IDs.
     */    
    public function getIds(): array
    {
        return array_map(function ($model) {
            return $model->id;
        }, (array) $this); // Cast to array to use array_map
    }

    /**
     * Serializes the collection to a JSON-serializable array.
     *
     * @return array The collection as an array.
     */    
    public function jsonSerialize(): array
    {
        return $this->getArrayCopy();
    }

    /**
     * Validates that a value is an instance of the expected model type.
     *
     * Checks that the given value is an instance of the class specified 
     * by the `$model` static property. Throws an exception if validation fails.
     *
     * @param mixed $value The value to validate.
     * @throws InvalidArgumentException If the value is not an instance of
     *                                  the expected model.
     */    
    protected function validateType($value): void
    {
        if (!$value instanceof static::$model) {
            throw new InvalidArgumentException(
                "Expected instance of " . static::$model
            );
        }
    }
}
