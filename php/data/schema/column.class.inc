<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

/**
 * Enum ColumnType
 *
 * Defines the possible column types for a database schema. These
 * correspond to common SQL data types used when defining tables.
 *
 * @package    LORIS
 * @subpackage Biobank
 */
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

/**
 * Class Column
 *
 * Represents a column in a database table. This class associates the
 * column with its name and the table it belongs to.
 *
 * @package    LORIS
 * @subpackage Biobank
 */
class Column
{
    public function __construct(
        public string $name,
        public Table $table,
    ) {}
}
