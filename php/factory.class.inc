<?php declare(strict_types=1); 

namespace LORIS\biobank;

use ReflectionClass;
use ReflectionNamedType;
use InvalidArgumentException;

class Factory
{
    public function __construct(
        private string $model
    ) {}

    public function create(array $data): object
    {
        $reflectionClass = new ReflectionClass($this->model);
        $constructor = $reflectionClass->getConstructor();
        $parameters = $constructor->getParameters();

        $args = [];
        foreach ($parameters as $parameter) {
            $name = $parameter->getName();
            $type = $parameter->getType();
            $typeName = $type ? $type->getName() : null;

            // error_log('MODEL: ' . $this->model);
            // error_log('NAME: ' . $name);
            if (isset($data[$name])) {
                $value = $data[$name];
                // error_log($name . ": " . print_r($value, true));

                // Type checking and casting
                $value = $this->handleType($type, $value);
                $args[] = $value;
            } else {
                if ($parameter->isOptional()) {
                    $args[] = $parameter->getDefaultValue();
                } else {
                    throw new InvalidArgumentException("Missing required parameter: {$name}");
                }
            }
        }

        return $reflectionClass->newInstanceArgs($args);
    }

    private function handleType(?ReflectionNamedType $type, $value)
    {
        if ($type) {
            $typeName = $type->getName();
            if ($type->isBuiltin()) {
                return $this->castToBuiltinType($typeName, $value);
            } elseif (class_exists($typeName) && !is_object($value)) {
                // Recursively instantiate nested objects
                $subFactory = new self($typeName);
                return $subFactory->create($value);
            } elseif (!$type->isBuiltin() && is_array($value)) {
                // Handle array of objects
                return array_map(function ($item) use ($typeName) {
                    if (class_exists($typeName) && !is_object($item)) {
                        $subFactory = new self($typeName);
                        return $subFactory->create($item);
                    }
                    return $item;
                }, $value);
            }
        }
        return $value;
    }

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
            default:
                throw new InvalidArgumentException("Unsupported built-in type: {$typeName}");
        }
    }

    // public static function createMany(array $models): Models
    // {
    //     $modelClass = get_class(reset($models));
    //     $collectionClass = $modelClass . 's'; // Assuming collection classes follow this naming convention

    //     if (!class_exists($collectionClass)) {
    //         throw new InvalidArgumentException(
    //             "Collection class does not exist: {$collectionClass}"
    //         );
    //     }

    //     return new $collectionClass($models);
    // }
}
