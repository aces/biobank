<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schemas\container;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\Schema;

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
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_container_type');
    }

    protected function getId(): Key
    {
        return new Key(name: 'ContainerTypeID');
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array {
        return [
            'label' => new Column(
                name: 'Label',
                type: ColumnType::VARCHAR
            ),
            // primary is a reserved word in sql
            'prime' => new Column( 
                name: 'Prime',
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

    protected function getRelations(): array {
        return [
            'capacity' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key(name: 'ContainerCapacityID')
                ),
                to: container\type\CapacitySchema::class,
                strategy: LoadingStrategy::EAGER
            ),
            'dimension' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key(name: 'ContainerDimensionID')
                ),
                to: container\type\DimensionSchema::class,
                strategy: LoadingStrategy::EAGER
            )
        ];
    }

    protected function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
