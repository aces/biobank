<?php declare(strict_types=1);

namespace LORIS\biobank\factories\container\type;

use LORIS\biobank\models\container\type\Capacity;
use LORIS\biobank\factories\UnitFactory;

class CapacityFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Capacity::class;
    }

    protected static function mapLazy(string $name, $value): mixed
    {
        return match ($name) {
            'id' => $value,
            'quantity' => $value,
            'unit' => UnitFactory::createLazy($value),
        };
    }

    protected static function mapEager(string $name, $value): mixed
    {
        return match ($name) {
            'id' => $value,
            'quantity' => $value,
            'unit' => UnitFactory::createEager($value),
        };
    }
}
