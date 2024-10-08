<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\shipment;

use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\Schema;

/**
 * Log Schema Class
 * 
 * This class defines the schema for the `shipment_log` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class LogSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\shipment\Log::class;                                 
    }  

    public static function table(): Table {
        return new Table(name: 'shipment_log');
    }

    public static function id(): string {
        return 'ShipmentID';
    }

    public static function label(): string {
        return '';
    }

    public static function columns(): array {
        return [
            'temperature' => new Column(
                name: 'Temperature',
                table: self::table()
            ),
            'date' => new Column(
                name: 'Date',
                table: self::table()
            ),
            'time' => new Column(
                name: 'Time',
                table: self::table()
            ),
            'comments' => new Column(
                name: 'Comments',
                table: self::table()
            ),
        ];
    }

    public static function relations(): array {
        return [
            'center' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'CenterID'
                ),
                to: schemas\CenterSchema::class,
            ),
            'status' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'StatusID'
                ),
                to: StatusSchema::class,
            ),
            'user' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'UserID'
                ),
                to: schemas\UserSchema::class,
            ),
        ];
    }

    public static function joins(): array {
        return []; // Add join definitions if needed
    }
}
