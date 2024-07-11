<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

enum ColumnType: string
{
    case INT = 'int';
    case VARCHAR = 'varchar';
    case DECIMAL = 'decimal';
    case BOOLEAN = 'boolean';
    case DATE = 'date';
    case TIME = 'time';
}

class Column
{
    public function __construct(
        public string $name,
        public ColumnType $type,
    ) {}
}
