<?php declare(strict_types=1);

namespace LORIS\biobank\factories;

use LORIS\biobank\models\Process;
use LORIS\biobank\factories\specimen\ProtocolFactory;
use LORIS\biobank\factories\CenterFactory;
use LORIS\biobank\factories\ExaminerFactory;
use LORIS\biobank\factories\specimen\DataFactory;

class ProcessFactory extends ModelFactory
{
    protected static function getModelClass(): string
    {
        return Process::class;
    }

    protected static function mapLazy(string $name, $value)
    {
        return match ($name) {
            'id' => $value,
            'protocol' => ProtocolFactory::build($value),
            'center' => CenterFactory::build($value),
            'examiner' => ExaminerFactory::build($value),
            'date' => new \DateTime($value),
            'time' => new \DateTime($value),
            'comments' => $value,
            'data' => DataFactory::build($value),
            default => $value,
        };
    }

    protected static function mapEager(string $name, $value)
    {
        return match ($name) {
            self::mapLazy($name, $value);
        };
    }
}
