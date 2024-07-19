<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\schemas\container;

/**
 * Container Schema Class
 *
 * This class defines the schema for the `biobank_container` table.
 *
 * PHP Version 7.2
 *
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */

class ContainerSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_container');
    }

    protected function getId(): Key
    {
        return new Key('ContainerID');
    }

    protected function getLabel(): string
    {
        return 'barcode';
    }

    protected function getColumns(): array
    {
        return [
            'barcode' => new Column(
                name: 'Barcode', 
                type: ColumnType::VARCHAR
            ),
            'temperature' => new Column(
                name: 'Temperature', 
                type: ColumnType::DECIMAL
            ),
            'lotNumber' => new Column(
                name: 'LotNumber', 
                type: ColumnType::VARCHAR
            ),
            'expirationDate' => new Column(
                name: 'ExpirationDate', 
                type: ColumnType::DATE
            ),
            'comments' => new Column(
                name: 'Comments', 
                type: ColumnType::VARCHAR
            ),
        ];
    }

    protected function getJoins(): array
    {
        return [
            'parent_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_container_parent', 
                        alias: 'parent_rel',
                    ),
                    key: $this->id
                )                
            ),
            'children_rel' => new Join(
                from: $this->idKey,
                to: new TableKey( 
                    table: new Table(
                        name: 'biobank_container_parent', 
                        alias: 'children_rel'
                    ),
                    key: new Key(name: 'ParentContainerID')
                ),
            ),
            'shipments_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_container_shipment_rel', 
                        alias: 'shipments_rel'
                    ),
                    key: $this->id
                )                
            )
        ];
    }

    protected function getRelations(): array
    {
        return [
            'type' => new Relation(
                from: new TableKey(
                    table: $this->table, 
                    key: new Key('ContainerTypeID')
                ),
                to: container\TypeSchema::class,
                strategy: LoadingStrategy::EAGER,
            ),
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table, 
                    key: new Key('CenterID')
                ),
                to: CenterSchema::class,
                strategy: LoadingStrategy::LAZY,
            ),
            'parent' => new Relation(
                from: new TableKey(
                    table: $this->joins['parent_rel']->to->table, 
                    key: new Key('ParentContainerID')
                ),
                to: self::class,
                strategy: LoadingStrategy::LAZY,
            ),
            'children' => new Relation(
                from: new TableKey(
                    table: $this->joins['children_rel']->to->table, 
                    key: new Key('ContainerID')
                ),
                to: self::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
           'shipments' => new Relation(
                from: new TableKey(
                    table: $this->joins['shipments_rel']->to->table, 
                    key: new Key('ShipmentID')
                ),
                to: ShipmentSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            )
        ];
    }
}
