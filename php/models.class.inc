<?php declare(strict_types=1);

namespace LORIS\biobank;

use ArrayObject;
use InvalidArgumentException;

// interface ModelsInterface
// {
//     public const MODEL = null;
// }

abstract class Models extends ArrayObject implements \JsonSerializable
{
    public static string $model;

    public function __construct(array $items = [])
    {
        parent::__construct([], ArrayObject::ARRAY_AS_PROPS);

        foreach ($items as $item) {
            $this->validateType($item);
            $this[] = $item;
        }
    }

    public function offsetSet($key, $value): void
    {
        $this->validateType($value);
        parent::offsetSet($key, $value);
    }

    public function append($value): void
    {
        $this->validateType($value);
        parent::append($value);
    }

    public function getIds(): array
    {
        return array_map(function ($model) {
            return $model->id;
        }, (array) $this); // Cast to array to use array_map
    }

    public function jsonSerialize(): array
    {
        return $this->getArrayCopy();
    }

    protected function validateType($value): void
    {
        if (!$value instanceof static::$model) {
            throw new InvalidArgumentException("Expected instance of " . static::$model);
        }
    }
}
