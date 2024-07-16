<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container\type;

use LORIS\biobank\data\schemas\UnitSchema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\AbstractSchema;

/**
 * Capacity Schema Class
 * 
 * This class defines the schema for the `biobank_container_capacity` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class CapacitySchema extends AbstractSchema
{
    public static function getTable(): Table
    {
        return new Table(name: 'biobank_container_capacity');
    }

    public static function getId(): Key
    {
        return new Key(name: 'ContainerCapacityID');
    }

    public static function getLabel(): string
    {
        return '';
    }

    public static function getColumns(): array
    {
        return [
            'quantity' => new Column(
                name: 'Quantity',
                type: ColumnType::DECIMAL
            ),
        ];
    }

    public static function getRelations(): array {
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key(name: 'UnitID')
                ),
                to: UnitSchema::eager()
            ),
        ];
    }

    public static function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
