<?php declare(strict_types=1);

namespace LORIS\biobank\factories\container\type;

use LORIS\biobank\models\container\type\Dimension;

class DimensionFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Dimension::class;
    }

    protected static function mapLazy(string $name, $value): mixed
    {
        return match ($name) {
            'id' => $value,
            'x' => $value,
            'xNumerical' => $value,
            'y' => $value,
            'yNumerical' => $value,
            'z' => $value,
            'zNumerical' => $value,
            default => $value, // Provide a default case
        };
    }

    protected static function mapEager(string $name, $value): mixed
    {
        return self::mapLazy($name, $value); // Use the same implementation as mapLazy
    }
}
