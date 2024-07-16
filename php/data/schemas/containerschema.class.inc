<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;
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

class ContainerSchema extends AbstractSchema
{
    public static function getTable(): schema\Table
    {
        return new schema\Table(name: 'biobank_container');
    }

    public static function getId(): schema\Key
    {
        return new schema\Key('ContainerID');
    }

    public static function getLabel(): string
    {
        return 'barcode';
    }

    public static function getColumns(): array
    {
        return [
            'barcode' => new schema\Column(
                name: 'Barcode', 
                type: schema\ColumnType::VARCHAR
            ),
            'temperature' => new schema\Column(
                name: 'Temperature', 
                type: schema\ColumnType::DECIMAL
            ),
            'lotNumber' => new schema\Column(
                name: 'LotNumber', 
                type: schema\ColumnType::VARCHAR
            ),
            'expirationDate' => new schema\Column(
                name: 'ExpirationDate', 
                type: schema\ColumnType::DATE
            ),
            'comments' => new schema\Column(
                name: 'Comments', 
                type: schema\ColumnType::VARCHAR
            ),
        ];
    }

    public static function getJoins(): array
    {
        return [
            'container_parent' => new schema\Join(
                from: self::getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_container_parent', 
                        alias: 'container_parent',
                    ),
                    key: self::getId()
                )                
            ),
            'container_children' => new schema\Join(
                from: self::getIdKey(),
                to: new schema\TableKey( 
                    table: new schema\Table(
                        name: 'biobank_container_parent', 
                        alias: 'container_children'
                    ),
                    key: new schema\Key(name: 'ParentContainerID')
                ),
            ),
            'container_shipments' => new schema\Join(
                from: self::getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_container_shipment_rel', 
                        alias: 'container_shipments'
                    ),
                    key: self::getId()
                )                
            )
        ];
    }

    public static function getRelations(): array
    {
        return [
            'type' => new schema\Relation(
                alias: 'type',
                from: new schema\TableKey(
                    table: self::getTable(), 
                    key: new schema\Key('ContainerTypeID')
                ),
                to: container\TypeSchema::eager()
            ),
            // 'center' => new schema\Relation(
            //     from: new schema\TableKey(
            //         table: self::getTable(), 
            //         key: new schema\Key('CenterID')
            //     ),
            //     to: CenterSchema::lazy()
            // ),
            'parent' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::getJoins()['container_parent']->to->table, 
                    key: new schema\Key('ParentContainerID')
                ),
                to: self::lazy(),
            ),
            'children' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::getJoins()['container_children']->to->table, 
                    key: new schema\Key('ContainerID')
                ),
                to: self::lazy(),
                many: true
            ),
            'shipments' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::getJoins()['container_shipments']->to->table, 
                    key: new schema\Key('ShipmentID')
                ),
                to: ShipmentSchema::lazy(),
                many: true
            )
        ];
    }
}
