<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data\schemas;                                     
                                                                                   
use \LORIS\biobank\data\schema\Table;                                               
use \LORIS\biobank\data\schema\TableKey;                                            
use \LORIS\biobank\data\schema\Column;                                              
use \LORIS\biobank\data\schema\ColumnType;                                          
use \LORIS\biobank\data\schema\Join;                                                
use \LORIS\biobank\data\Schema;                                             
                                                                                   
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
                                                                                   
class UserSchema extends Schema                                          
{                                                                                  
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\User::class;                              
    }  

    protected function getTable(): Table {                                            
        return new Table(name: 'users');                                 
    }                                                                              
                                                                                   
    protected function getId(): string {                                                 
        return 'ID';                                          
    }                                                                              
                                                                                   
    protected function getLabel(): string {                                           
        return 'name';                                                            
    }                                                                              
                                                                                   
    protected function getColumns(): array {                                          
        return [                                                                   
            'name' => new Column(                                                 
                name: 'Real_name',                                                     
                table: $this->table,
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
