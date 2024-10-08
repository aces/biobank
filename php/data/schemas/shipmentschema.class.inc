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

use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\schemas\shipment;
use \LORIS\biobank\data\Schema;

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

class ShipmentSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Shipment::class;                              
    }  

    public static function table(): Table {
        return new Table(name: 'shipment');
    }

    public static function id(): string {
        return 'ShipmentID';
    }

    public static function label(): string {
        return 'barcode';
    }

    public static function columns(): array {
        return [
            'barcode' => new Column(
                name: 'Barcode',
                table: self::table()
            ),
        ];
    }

    public static function joins(): array {
        return [
            'shipment_containers' => new Join(
                from: new TableKey(
                    table: self::table(),
                    key: self::id()
                ),
                to: new TableKey(
                    table: new Table(name: 'biobank_container_shipment_rel'),
                    key: self::id()
                )
            ),
        ];
    }

    public static function relations(): array {
        return [
            'type' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'ShipmentTypeID'
                ),
                to: shipment\TypeSchema::class,
            ),
            'destinationCenter' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'DestinationCenterID'
                ),
                to: CenterSchema::class,
            ),
            'logs' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: self::id()
                ),
                to: shipment\LogSchema::class,
                many: true,
            ),
            'containers' => new Relation(
                from: new TableKey(
                    table: self::joins()['shipment_containers']->to->table,
                    key: 'ContainerID'
                ),
                to: ContainerSchema::class,
                many: true,
            ),
        ];
    }
}
