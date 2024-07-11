<?php

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

            if (isset($data[$name])) {
                $value = $data[$name];

                // Type checking and casting
                if ($typeName) {
                    if ($type->isBuiltin()) {
                        $value = $this->castToBuiltinType($typeName, $value);
                    } elseif (class_exists($typeName) && !is_object($value)) {
                        // Recursively instantiate nested objects
                        $value = $this->create($typeName, $value);
                    } elseif ($type->isBuiltin() === false && is_array($value)) {
                        // Handle array of objects
                        $value = array_map(fn($item) => $this->create($typeName, $item), $value);
                    }
                }
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
}
