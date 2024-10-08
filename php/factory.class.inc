<?php 

namespace LORIS\biobank;

use ReflectionClass;
use ReflectionNamedType;
use InvalidArgumentException;
use LORIS\biobank\Model;
use LORIS\biobank\Models;

/**
 * Class Factory
 *
 * Handles the creation of model instances from arrays of data.
 *
 * @category   Factory
 * @package    Biobank
 * @subpackage Services
 * @author     Henri Rabalais <henri.rabalais@mcgill.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 */
class Factory
{
    /**
     * Constructs the Factory with a model class name.
     *
     * @param string $model The name of the model class.
     */    
    public function __construct(
        private string $model
    ) {}

    /**
     * Creates an instance of the model class with provided data.
     *
     * @param array|null $data Data to instantiate the model.
     * @return object          The instantiated model object.
     */
    public function create(?array $data): object
    {
        // error_log("Factory::create() called with data: " . print_r($data, true));

        $reflectionClass = new ReflectionClass($this->model);
        $constructor = $reflectionClass->getConstructor();
        $parameters = $constructor->getParameters();
    
        $args = array_map(function ($parameter) use ($data) {
            $name = $parameter->getName();
            $type = $parameter->getType();
            $typeName = $type ? $type->getName() : null;
            // error_log("Processing parameter: {$name}, type: {$typeName}");
    
            // Type checking and casting
            if (isset($data[$name])) {
                // error_log("Data for parameter '{$name}' found: " . print_r($data[$name], true));
                return $this->handleType($type, $data[$name]);
            }

            // Use default value if parameter is optional
            if ($parameter->isOptional()) {
                // error_log("Parameter '{$name}' is optional, using default value.");
                return $parameter->getDefaultValue();
            }

            // Handle nullable parameters explicitly
            if ($type->allowsNull()) {
                // error_log("Parameter '{$name}' allows null, returning null.");
                return null;
            }
    
            throw new InvalidArgumentException("Missing required parameter: {$name}");
        }, $parameters);
    
        return $reflectionClass->newInstanceArgs($args);
    }    

    /**
     * Creates multiple instances of the model class from a list of data arrays.
     *
     * @param array $dataList List of data arrays to instantiate the model.
     * @return array          List of instantiated model objects.
     */    
    public function createFromList(array $dataList): array
    {
        $instances = [];

        foreach ($dataList as $data) {
            try {
                $instances[] = $this->create($data);
            } catch (Exception $e) {
                // Handle the exception or log it
                throw new RuntimeException("Error creating instance: " . $e->getMessage());
            }
        }

        return $instances;
    }

    /**
     * Handles type conversion and instantiation for the provided value.
     *
     * @param ReflectionNamedType|null $type  The type of the parameter.
     * @param mixed                    $value The value to be handled.
     * @return mixed                          The processed value.
     */
    private function handleType(?ReflectionNamedType $type, $value)
    {
        // error_log("handleType() called with type: " . ($type ? $type->getName() : 'null') . ", value: " . print_r($value, true));
        if (!$type) {
            // error_log("No type provided, returning value as is.");
            return $value;
        }
    
        $typeName = $type->getName();

        // Special handling for ValidatableIdentifier subclasses
        // XXX: I don't particularly like this and I think they should accept
        // ints as valid
        if (is_subclass_of($typeName, \ValidatableIdentifier::class) && is_int($value)) {
            // error_log("Type '{$typeName}' is a ValidatableIdentifier subclass and value is int. Converting to string.");
            return new $typeName((string)$value);
        }
    
        // Handle built-in types
        if ($type->isBuiltin()) {
            // error_log("Type '{$typeName}' is built-in. Casting value.");
            return $this->castToBuiltinType($typeName, $value);
        }
    
        // Handle class types
        if (!class_exists($typeName)) {
            throw new InvalidArgumentException("Unable to handle type $typeName: $type");
        }
    
        if (is_subclass_of($typeName, Model::class)) {
            // error_log("Type '{$typeName}' is a subclass of Model.");
            $subFactory = new self($typeName);
            return $subFactory->create($value);
        }
    
        // Handle classes extending Models
        if (is_subclass_of($typeName, Models::class)) {
            // error_log("Type '{$typeName}' is a subclass of Models.");
            $subFactory = new self($typeName::$model);
            return new $typeName(array_map([$subFactory, 'create'], $value));
        }
    
        // Directly instantiate non-Model classes
        // error_log("Type '{$typeName}' is not a built-in or Model subclass. Instantiating directly.");
        return new $typeName($value);        
    }

    /**
     * Casts the provided value to the specified built-in type.
     *
     * @param string $typeName The name of the type to cast to.
     * @param mixed  $value    The value to be casted.
     * @return mixed           The casted value.
     * @throws InvalidArgumentException If the type is not supported.
     */    
    private function castToBuiltinType(string $typeName, $value)
    {
        switch ($typeName) {
            case 'int':
                return (int) $value;
            case 'float':
                return (float) $value;
            case 'string':
                return (string) $value;
            case 'bool':
                return (bool) $value;
            case 'array':
                return (array) $value;
            default:
                throw new InvalidArgumentException("Unsupported built-in type: {$typeName}");
        }
    }
}
