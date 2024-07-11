<?php declare(strict_types=1);

namespace LORIS\biobank\factories;

use LORIS\biobank\models\Analysis;
use LORIS\biobank\factories\ProcessFactory;

class AnalysisFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Analysis::class;
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
