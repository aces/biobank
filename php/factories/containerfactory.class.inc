<?php declare(strict_types=1); 

namespace LORIS\biobank\factories;

use LORIS\biobank\models\Container;
use LORIS\biobank\factories\container\TypeFactory;
use LORIS\biobank\factories\ShipmentFactory;
use LORIS\biobank\factories\CenterFactory;
use \DateTime;

class ContainerFactory extends ModelFactory
{
    protected static function getModelClass(): string {
        return Container::class;
    }

    protected static function mapLazy(string $name, $value)
    {
        return match ($name) {
            'id' => $value,
            'barcode' => $value,
        };
    }

    protected static function mapEager(string $name, $value)
    {
        return match ($name) {
            'id' => $value,
            'barcode' => $value,
            'type' => TypeFactory::buildEager($value),
            'temperature' => $value,
            'shipments' => ShipmentFactory::build($value), // Lazy creation for related models
            'center' => CenterFactory::build($value), // Lazy creation for related models
            'parent' => self::build($value), // Lazy creation for related models
            'children' => ContainerFactory::buildMany($value), // Lazy creation for related models
            'coordinate' => $value,
            'lotNumber' => $value,
            'expirationDate' => new DateTime($value),
            'comments' => $value,
            'date' => new DateTime($value),
        };
    }
}
