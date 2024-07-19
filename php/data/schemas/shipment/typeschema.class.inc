<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data\schemas\shipment;                                     
                                                                                   
use LORIS\biobank\data\schema\Table;                                               
use LORIS\biobank\data\schema\Key;                                                 
use LORIS\biobank\data\schema\Column;                                              
use LORIS\biobank\data\schema\ColumnType;                                          
use LORIS\biobank\data\Schema;                                             
                                                                                   
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
    protected function getTable(): Table {                                            
        return new Table(name: 'shipment_type');                                 
    }                                                                              
                                                                                   
    protected function getId(): Key {                                                 
        return new Key(name: 'ShipmentTypeID');                                          
    }                                                                              
                                                                                   
    protected function getLabel(): string {                                           
        return 'label';                                                            
    }                                                                              
                                                                                   
    protected function getColumns(): array {                                          
        return [                                                                   
            'label' => new Column(                                                 
                name: 'Label',                                                     
                type: ColumnType::VARCHAR                                          
            )                                                                      
        ];                                                                         
    }                                                                              
                                                                                   
    protected function getRelations(): array {                                        
        return [];                                                                 
    }                                                                              
                                                                                   
    protected function getJoins(): array {                                            
        return [];                                                                 
    }                                                                              
}      
