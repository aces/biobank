<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container;

use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Key;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schemas\container;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\Schema;

/**
 * Type Schema Class
 *
 * This class defines the schema for the `biobank_container_type` table.
 *
 * PHP Version 7.2
 *
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class TypeSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\container\Type::class;                              
    }  

    public static function table(): Table
    {
        return new Table(name: 'biobank_container_type');
    }

    public static function id(): string
    {
        return 'ContainerTypeID';
    }

    public static function label(): string
    {
        return 'label';
    }

    public static function columns(): array {
        return [
            'label' => new Column(
                name: 'Label',
                table: self::table()
            ),
            // primary is a reserved word in sql
            'prime' => new Column( 
                name: 'Prime',
                table: self::table()
            ),
            'brand' => new Column(
                name: 'Brand',
                table: self::table()
            ),
            'productNumber' => new Column(
                name: 'ProductNumber',
                table: self::table()
            )
        ];
    }

    public static function relations(): array {
        return [
            'capacity' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'ContainerCapacityID'
                ),
                to: container\type\CapacitySchema::class,
            ),
            'dimension' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'ContainerDimensionID'
                ),
                to: container\type\DimensionSchema::class,
            )
        ];
    }

    public static function joins(): array {
        return []; // Add join definitions if needed
    }
}
