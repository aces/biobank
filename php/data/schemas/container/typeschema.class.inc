<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\AbstractSchema;

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
final class TypeSchema extends AbstractSchema
{
    public static function getTable(): Table
    {
        return new Table(name: 'biobank_container_type');
    }

    public static function getId(): Key
    {
        return new Key(name: 'ContainerTypeID');
    }

    public static function getLabel(): string
    {
        return 'label';
    }

    public static function getColumns(): array {
        return [
            'label' => new Column(
                name: 'Label',
                type: ColumnType::VARCHAR
            ),
            'primary' => new Column(
                name: 'Primary',
                type: ColumnType::BOOLEAN
            ),
            'brand' => new Column(
                name: 'Brand',
                type: ColumnType::VARCHAR
            ),
            'productNumber' => new Column(
                name: 'ProductNumber',
                type: ColumnType::VARCHAR
            )
        ];
    }

    public static function getRelations(): array {
        return [
            'capacity' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key(name: 'ContainerCapacityID')
                ),
                to: \LORIS\biobank\data\type\CapacitySchema::eager()
            ),
            'dimension' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key(name: 'ContainerDimensionID')
                ),
                to: \LORIS\biobank\data\type\DimensionSchema::eager()
            )
        ];
    }

    public static function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
