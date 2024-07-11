<?php declare(strict_types=1);

namespace LORIS\biobank\factories;

use LORIS\biobank\models\specimen\Collection;
use LORIS\biobank\models\specimen\Process;

class CollectionFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Collection::class;
    }

    protected static function mapLazy(string $name, $value)
    {
        return match ($name) {
            'quantity' => new DecimalType($value)->getValue(),
            'unit' => UnitType::fromArray($value),
            'process' => ProcessFactory::build($value),
            default => $value,
        };
    }

    protected static function mapEager(string $name, $value)
    {
        return match ($name) {
            'quantity' => new DecimalType($value)->getValue(),
            'unit' => UnitType::fromArray($value),
            'process' => ProcessFactory::buildEager($value),
            default => $value,
        };
    }
}
