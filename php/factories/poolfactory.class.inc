<?php declare(strict_types=1);

namespace LORIS\biobank\factories;

use LORIS\biobank\models\Pool;
use LORIS\biobank\factories\UnitFactory;
use LORIS\biobank\factories\SpecimensFactory;
use LORIS\biobank\factories\CandidateFactory;
use LORIS\biobank\factories\SessionFactory;
use LORIS\biobank\factories\container\TypeFactory;
use LORIS\biobank\factories\CenterFactory;
use LORIS\biobank\factories\ProjectFactory;
use LORIS\biobank\types\Date;
use LORIS\biobank\types\Time;

class PoolFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Pool::class;
    }

    protected static function mapLazy(string $name, $value)
    {
        return match ($name) {
            'id' => $value,
            'label' => $value,
            'quantity' => $value,
            'unit' => UnitFactory::build($value),
            'specimens' => SpecimenFactory::buildMany($value), // Lazy creation for related models
            'date' => new DateTime($value),
            'time' => new DateTime($value),
        };
    }
}
