<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data\schemas\shipment;                                     
                                                                                   
use \LORIS\biobank\data\schema\Table;                                               
use \LORIS\biobank\data\schema\Column;                                              
use \LORIS\biobank\data\schema\ColumnType;                                          
use \LORIS\biobank\data\Schema;                                             
                                                                                   
/**                                                                                
 * Shipment Type Schema Class                                                    
 *                                                                                 
 * This class defines the schema for the `shipment_type` table.                  
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
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\shipment\Type::class;                            
    }   

    public static function table(): Table {                                            
        return new Table(name: 'shipment_type');                                 
    }                                                                              
                                                                                   
    public static function id(): string {                                                 
        return 'ShipmentTypeID';                                          
    }                                                                              
                                                                                   
    public static function label(): string {                                           
        return 'label';                                                            
    }                                                                              
                                                                                   
    public static function columns(): array {                                          
        return [                                                                   
            'label' => new Column(                                                 
                name: 'Label',                                                     
                table: self::table()
            )                                                                      
        ];                                                                         
    }                                                                              
                                                                                   
    public static function relations(): array {                                        
        return [];                                                                 
    }                                                                              
                                                                                   
    public static function joins(): array {                                            
        return [];                                                                 
    }                                                                              
}      
