<?php declare(strict_types=1);

namespace LORIS\biobank\factories;

use LORIS\biobank\models\Preparation;
use LORIS\biobank\factories\ProcessFactory;

class PreparationFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Preparation::class;
    }

    protected static function mapLazy(string $name, $value)
    {
        return ProcessFactory::mapLazy($name, $value);
    }

    protected static function mapEager(string $name, $value)
    {
        return ProcessFactory::mapEager($name, $value);
    }
}
