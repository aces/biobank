<?php declare(strict_types=1);

namespace LORIS\biobank;

use ArrayObject;
use InvalidArgumentException;

// interface ModelsInterface
// {
//     public const MODEL = null;
// }

abstract class Models extends ArrayObject 
{
    public static string $model;

    public function __construct(array $items = [])
    {
        parent::__construct([], ArrayObject::ARRAY_AS_PROPS);

        foreach ($items as $item) {
            // if (!$item instanceof static::MODEL) {
            //     throw new InvalidArgumentException("Expected instance of ".static::MODEL);
            // }
            $this[] = $item;
        }
    }

    public function offsetSet($key, $value): void
    {
        // if (!$value instanceof static::MODEL) {
        //     throw new InvalidArgumentException("Expected instance of ".static::MODEL);
        // }
        parent::offsetSet($key, $value);
    }

    public function append($value): void
    {
        // if (!$value instanceof static::MODEL) {
        //     throw new InvalidArgumentException("Expected instance of ".static::MODEL);
        // }
        parent::append($value);
    }
}
