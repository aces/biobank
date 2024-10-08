<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

/**
 * Diagnosis Schema Class
 * 
 * This class defines the schema for the `diagnosis` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class DiagnosisSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Diagnosis::class;                              
    }  

    public static function table(): Table                                           
    {                                                                              
        return new Table(name: 'diagnosis');                                    
    }      

    public static function id(): string                                                
    {                                                                              
        return 'DiagnosisID';                                                  
    }                                                                              
                                                                                   
    public static function label(): string                                          
    {                                                                              
        return 'label';                                                            
    }                                                                              
                                                                                   
    public static function columns(): array                                         
    {                                                                              
        return [                                                                   
            'label' => new Column(                                                 
                name: 'Name',                                                     
                table: self::table()
            ),                                                                     
            'abbreviation' => new Column(
                name: 'Abbreviation',
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
        return [];                                                                 
    }       
}
