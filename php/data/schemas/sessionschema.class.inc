<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\Schema;

/**
 * Session Schema Class
 * 
 * This class defines the schema for the `session` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class SessionSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Session::class;                              
    }  

    public static function table(): Table                                       
    {                                                                              
        return new Table(name: 'session');
    }                                                                              
                                                                                   
    public static function id(): string                                            
    {                                                                              
        return 'ID';                                                  
    }                                                                              
                                                                                   
    public static function label(): string                                      
    {                                                                              
        return 'label';                                                            
    }                                                                              
                                                                                   
    public static function columns(): array                                     
    {                                                                              
        return [                                                                   
            'label' => new Column(
                name: 'Visit_label',
                table: self::table()
            ),
        ];                                                                         
    }                                                                              
                                                                                   
    public static function joins(): array                                       
    {                                                                              
        return [];                                                                 
    }                                                                              
                                                                                   
    public static function relations(): array                                   
    {                                                                              
        return [
            'candidate' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: 'CandID'
                ),
                to: schemas\CandidateSchema::class,
                many: true,
            ),
            'center' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::table(),
                    key: 'CenterID'
                ),
                to: schemas\CenterSchema::class,
            ),
        ];
    }                                                                              
}    
