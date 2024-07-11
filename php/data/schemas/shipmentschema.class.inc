<?php declare(strict_types=1);

/**
 * Shipment Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Shipment Object instances
 *
 * PHP Version 7.2
 *
 * @category   DAO
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\AbstractSchema;

/**
 * Shipment Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Shipment Object instances
 *
 * PHP Version 7.2
 *
 * @category   DAO
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */

class ShipmentSchema extends AbstractSchema
{
    public static function getTable(): Table {
        return new Table(name: 'shipment');
    }

    public static function getId(): Key {
        return new Key(name: 'ShipmentID');
    }

    public static function getLabel(): string {
        return 'barcode';
    }

    public static function getColumns(): array {
        return [
            'barcode' => new Column(
                name: 'Barcode',
                type: ColumnType::VARCHAR
            ),
        ];
    }

    public static function getJoins(): array {
        return [
            'shipment_containers' => new Join(
                from: self::getIdKey(),
                to: new TableKey(
                    table: new Table(name: 'biobank_container_shipment_rel'),
                    key: self::getId()
                )
            ),
        ];
    }

    public static function getRelations(): array {
        return [
            'type' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key(name: 'ShipmentTypeID')
                ),
                to: ShipmentTypeSchema::eager()
            ),
            'destinationCenter' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key(name: 'DestinationCenterID')
                ),
                to: CenterSchema::lazy()
            ),
            'containers' => new Relation(
                from: new TableKey(
                    table: new Table(name: 'biobank_container_shipment_rel'),
                    key: new Key(name: 'ContainerID')
                ),
                to: ContainerSchema::lazy(),
                many: true
            ),
        ];
    }
}
