<?php declare(strict_types=1); 

namespace LORIS\biobank;

use ReflectionClass;
use InvalidArgumentException;

abstract class ModelFactory
{
    abstract protected static function getModelClass(): string;
    abstract protected static function mapyLazy(string $name, $value);
    abstract protected static function mapEager(string $name, $value);

    public static function create(array $data): object
    {
        return static::mapData($data, [static::class, 'map']); // Lazy loading by default
    }

    public static function createMany(array $data): array
    {
        return array_map(fn($item) => self::create($item), $data);
    }

    public static function buildManyEager(array $data): array
    {
        return array_map(fn($item) => self::buildEager($item), $data);
    }

    protected static function mapData(array $data, function $mapFunction): object
    {
        $modelClass = static::getModelClass();
        $reflectionClass = new ReflectionClass($modelClass);
        $constructor = $reflectionClass->getConstructor();
        $parameters = $constructor->getParameters();

        $args = [];

        foreach ($parameters as $parameter) {
            $name = $parameter->getName();
            if (array_key_exists($name, $data)) {
                // Check if the key exists in the array
                $args[] = $mapFunction($name, $data[$name]);
            } else {
                // If the key does not exist, do not set it
                $args[] = $parameter->isOptional() ? $parameter->getDefaultValue() : null;
            }
        }

        return $reflectionClass->newInstanceArgs($args);
    }

    function validateData($data) {
    $orderValidator = v::key('id', v::intVal())
        ->key('customer', v::key('id', v::intVal())
            ->key('name', v::stringType())
            ->key('address', v::key('street', v::stringType())
                ->key('city', v::stringType())
                ->key('zipCode', v::stringType())))
        ->key('products', v::arrayType()->each(v::key('id', v::intVal())
            ->key('name', v::stringType())
            ->key('price', v::floatType())));
    }

    protected static function cast(string $name, $value, string $type)
    {
        switch ($type) {
            case 'int':
                if (is_numeric($value) && (int) $value == $value) {
                    return (int) $value;
                }
                break;
            case 'float':
                if (is_numeric($value) && (float) $value == $value) {
                    return (float) $value;
                }
                break;
            case 'bool':
                if (in_array($value, [true, false, 0, 1, '0', '1'], true)) {
                    return filter_var($value, FILTER_VALIDATE_BOOLEAN, FILTER_NULL_ON_FAILURE);
                }
                break;
            case 'array':
                if (is_array($value)) {
                    return $value;
                }
                break;
            case 'string':
                if (is_string($value)) {
                    return $value;
                }
                break;
            default:
                if ($value instanceof $type) {
                    return $value;
                }
                break;
        }

        throw new InvalidArgumentException("Invalid type for $name: expected $type");
    }

    protected static function validate(string $name, $value, string $type): void
    {
        switch ($type) {
            case 'int':
                if (!is_int($value)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected int");
                }
                break;
            case 'float':
                if (!is_float($value) && !is_numeric($value)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected float");
                }
                break;
            case 'string':
                if (!is_string($value)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected string");
                }
                break;
            case 'bool':
                if (!is_bool($value)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected bool");
                }
                break;
            case 'array':
                if (!is_array($value)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected array");
                }
                break;
            default:
                if (!($value instanceof $type)) {
                    throw new InvalidArgumentException("Invalid type for $name: expected instance of $type");
                }
                break;
        }
    }
}
