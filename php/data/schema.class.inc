<?php declare(strict_types=1);  

namespace LORIS\biobank\data;

use \LORIS\biobank\data\schema\Table;

/**
 * Abstract class Schema
 *
 * Defines the structure of a schema, including its model, table, columns,
 * joins, and relationships. Concrete schema classes must implement these
 * methods to specify how data is mapped and interacted with in the system.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @abstract
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class Schema
{
    /**
     * Returns the fully qualified class name of the associated model.
     *
     * Concrete classes should implement this method to specify the model
     * that the schema corresponds to.
     *
     * @return string The class name of the model.
     */    
    abstract public static function model(): string;

    /**
     * Returns the table associated with the schema.
     *
     * Concrete classes should implement this method to return the table
     * that the schema maps to.
     *
     * @return Table The database table object.
     */    
    abstract public static function table(): Table;

    /**
     * Returns the name of the primary key column.
     *
     * Concrete classes should implement this method to specify the name of
     * the primary key field used in the table.
     *
     * @return string The primary key column name.
     */    
    abstract public static function id(): string;

    /**
     * Returns the name of the label column.
     *
     * Concrete classes should implement this method to specify the label
     * column, which typically represents a human-readable identifier.
     *
     * @return string The label column name.
     */    
    abstract public static function label(): string;

    /**
     * Returns an array of column definitions for the table.
     *
     * Concrete classes should implement this method to provide an array of
     * columns, each defining the fields present in the table.
     *
     * @return array The column definitions as an associative array.
     */    
    abstract public static function columns(): array;

    /**
     * Returns an array of join definitions.
     *
     * Concrete classes should implement this method to define how this
     * schema joins with other tables or schemas in the database.
     *
     * @return array The join definitions as an associative array.
     */    
    abstract public static function joins(): array;

    /**
     * Returns an array of relation definitions.
     *
     * Concrete classes should implement this method to define relationships
     * between this schema and other models or schemas.
     *
     * @return array The relation definitions as an associative array.
     */    
    abstract public static function relations(): array;
}
