<?php declare(strict_types=1);

namespace LORIS\biobank;

use ArrayObject;
use InvalidArgumentException;

class Models extends ArrayObject
{
    protected string $modelClass;

    public function __construct(array $items = [])
    {
        parent::__construct([], ArrayObject::ARRAY_AS_PROPS);

        foreach ($items as $item) {
            if (!$item instanceof $this->modelClass) {
                throw new InvalidArgumentException("Expected instance of {$this->modelClass}");
            }
            $this[] = $item;
        }
    }

    public function offsetSet($key, $value): void
    {
        if (!$value instanceof $this->modelClass) {
            throw new InvalidArgumentException("Expected instance of {$this->modelClass}");
        }
        parent::offsetSet($key, $value);
    }

    public function append($value): void
    {
        if (!$value instanceof $this->modelClass) {
            throw new InvalidArgumentException("Expected instance of {$this->modelClass}");
        }
        parent::append($value);
    }
}
