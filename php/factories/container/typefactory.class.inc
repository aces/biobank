<?php declare(strict_types=1);

namespace LORIS\biobank\factories\container;

use LORIS\biobank\models\container\Type;
use LORIS\biobank\factories\CapacityFactory;
use LORIS\biobank\factories\DimensionFactory;

class TypeFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Type::class;
    }

    protected static function mapLazy(string $name, $value): mixed
    {
        return match ($name) {
            'id' => $value,
            'label' => $value,
            'brand' => $value,
            'productNumber' => $value,
            'primary' => $value,
            'capacity' => CapacityFactory::create($value),
            'dimension' => DimensionFactory::create($value),
        };
    }

    protected static function mapEager(string $name, $value): mixed
    {
        return match ($name) {
            'id' => $value,
            'label' => $value,
            'brand' => $value,
            'productNumber' => $value,
            'primary' => $value,
            'capacity' => CapacityFactory::createEager($value),
            'dimension' => DimensionFactory::createEager($value),
        };
    }
}
