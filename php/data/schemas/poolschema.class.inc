<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\LoadingStrategy;

/**
 * Pool Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Pool Object instances
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

class PoolSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Pool::class;                              
    }  

    public static function table(): Table
    {
        return new Table(name: 'biobank_pool');
    }

    public static function id(): string
    {
        return 'PoolID';
    }

    public static function label(): string                                      
    {                                                                              
        return 'label';                                                          
    }   

    public static function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Label', 
                table: self::table()
            ),
            'quantity' => new Column(
                name: 'Quantity', 
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
        ];
    }

    public static function joins(): array
    {
        return [
            'specimen_rel' => new Join(
                from: new TableKey(
                    table: self::table(),
                    key: self::id(),
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_pool_rel', 
                    ),
                    key: self::id()
                )
            )
        ];
    }

    public static function relations(): array
    {
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'UnitID'
                ),
                to: UnitSchema::class,
            ),
            'specimens' => new Relation(
                from: new TableKey(
                    table: self::joins()['specimen_rel']->to->table,
                    key: 'SpecimenID'
                ),
                to: SpecimenSchema::class,
                many: true
            ),
        ];
    }
}
