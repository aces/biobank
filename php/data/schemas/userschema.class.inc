<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data\schemas;                                     
                                                                                   
use LORIS\biobank\data\schema\Table;                                               
use LORIS\biobank\data\schema\Key;                                                 
use LORIS\biobank\data\schema\TableKey;                                            
use LORIS\biobank\data\schema\Column;                                              
use LORIS\biobank\data\schema\ColumnType;                                          
use LORIS\biobank\data\schema\Relation;                                            
use LORIS\biobank\data\schema\Join;                                                
use LORIS\biobank\data\AbstractSchema;                                             
                                                                                   
/**                                                                                
 * User Schema Class                                                    
 *                                                                                 
 * This class defines the schema for the `users' table.                  
 *                                                                                 
 * PHP Version 7.2                                                                 
 *                                                                                 
 * @category   Data                                                                
 * @package    Loris                                                               
 * @subpackage Biobank                                                             
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3                       
 * @link       https://www.github.com/aces/Loris/                                  
 */                                                                                
                                                                                   
class UserSchema extends AbstractSchema                                          
{                                                                                  
    public function getTable(): Table {                                            
        return new Table(name: 'users');                                 
    }                                                                              
                                                                                   
    public function getId(): Key {                                                 
        return new Key(name: 'ID');                                          
    }                                                                              
                                                                                   
    public function getLabel(): string {                                           
        return 'name';                                                            
    }                                                                              
                                                                                   
    public function getColumns(): array {                                          
        return [                                                                   
            'name' => new Column(                                                 
                name: 'Real_name',                                                     
                type: ColumnType::VARCHAR                                          
            )                                                                      
        ];                                                                         
    }                                                                              
                                                                                   
    public function getRelations(): array {                                        
        return [];                                                                 
    }                                                                              
                                                                                   
    public function getJoins(): array {                                            
        return [];                                                                 
    }                                                                              
}      
