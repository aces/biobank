<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

enum ColumnType: string
{
    case INT = 'int';
    case TINYINT = 'tinyint';
    case VARCHAR = 'varchar';
    case LONGTEXT = 'longtext';
    case DECIMAL = 'decimal';
    case BOOLEAN = 'boolean';
    case ENUM = 'enum';
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
