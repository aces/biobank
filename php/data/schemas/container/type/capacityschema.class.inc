<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container\type;

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
class CapacitySchema extends AbstractSchema
{
    public function getTable(): Table {
        return new Table(name: 'biobank_container_capacity');
    }

    public function getId(): Key {
        return new Key(name: 'ContainerCapacityID');
    }

    public function getLabel(): string {
        return '';
    }

    public function getColumns(): array {
        return [
            'quantity' => new Column(
                name: 'Quantity',
                type: ColumnType::DECIMAL
            ),
        ];
    }

    public function getRelations(): array {
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->getTable(),
                    key: new Key(name: 'UnitID')
                ),
                to: LORIS\biobank\data\schemas\UnitSchema::eager()
            ),
        ];
    }

    public function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
