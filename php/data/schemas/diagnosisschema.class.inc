<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

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
    protected function getTable(): Table                                           
    {                                                                              
        return new Table(name: 'diagnosis');                                    
    }      

    protected function getId(): Key                                                
    {                                                                              
        return new Key('DiagnosisID');                                                  
    }                                                                              
                                                                                   
    protected function getLabel(): string                                          
    {                                                                              
        return 'label';                                                            
    }                                                                              
                                                                                   
    protected function getColumns(): array                                         
    {                                                                              
        return [                                                                   
            'label' => new Column(                                                 
                name: 'Name',                                                     
                type: ColumnType::VARCHAR                                          
            ),                                                                     
            'abbreviation' => new Column(
                name: 'Abbreviation',
                type: ColumnType::VARCHAR
            ),
        ];                                                                         
    }                                                                              
                                                                                   
    protected function getJoins(): array                                           
    {                                                                              
        return [];                                                                 
    }                                                                              
                                                                                   
    protected function getRelations(): array                                       
    {                                                                              
        return [];                                                                 
    }       
}
