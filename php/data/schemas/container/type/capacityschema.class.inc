<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container\type;

use \LORIS\biobank\data\schemas\UnitSchema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Key;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\Schema;

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
final class CapacitySchema extends Schema
{

    protected function getModel(): string
    {
        return \LORIS\biobank\models\container\type\Capacity::class;
    }

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_container_capacity');
    }

    protected function getId(): string
    {
        return 'ContainerCapacityID';
    }

    protected function getLabel(): string
    {
        return '';
    }

    protected function getColumns(): array
    {
        return [
            'quantity' => new Column(
                name: 'Quantity',
                table: $this->table,
            ),
        ];
    }

    protected function getRelations(): array {
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'UnitID'
                ),
                to: UnitSchema::class,
                type: RelationType::ManytoOne,
            ),
        ];
    }

    protected function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
