<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\schemas\container;

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
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Container::class;                              
    }  

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_container');
    }

    protected function getId(): string
    {
        return 'ContainerID';
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
                table: $this->table,
            ),
            'temperature' => new Column(
                name: 'Temperature', 
                table: $this->table,
            ),
            'lotNumber' => new Column(
                name: 'LotNumber', 
                table: $this->table,
            ),
            'expirationDate' => new Column(
                name: 'ExpirationDate', 
                table: $this->table,
            ),
            'comments' => new Column(
                name: 'Comments', 
                table: $this->table,
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
                    key: 'ParentContainerID'
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
                    key: 'ContainerTypeID'
                ),
                to: container\TypeSchema::class,
            ),
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table, 
                    key: 'CenterID'
                ),
                to: CenterSchema::class,
            ),
            'parent' => new Relation(
                from: new TableKey(
                    table: $this->joins['parent_rel']->to->table, 
                    key: 'ParentContainerID'
                ),
                to: self::class,
            ),
            'children' => new Relation(
                from: new TableKey(
                    table: $this->joins['children_rel']->to->table, 
                    key: 'ContainerID'
                ),
                to: self::class,
                many: true,
            ),
           'shipments' => new Relation(
                from: new TableKey(
                    table: $this->joins['shipments_rel']->to->table, 
                    key: 'ShipmentID'
                ),
                to: ShipmentSchema::class,
                many: true,
            )
        ];
    }
}
