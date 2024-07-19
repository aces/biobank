<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\Schema;

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
    protected function getTable(): Table                                       
    {                                                                              
        return new Table(name: 'session');
    }                                                                              
                                                                                   
    protected function getId(): Key                                            
    {                                                                              
        return new Key('ID');                                                  
    }                                                                              
                                                                                   
    protected function getLabel(): string                                      
    {                                                                              
        return 'label';                                                            
    }                                                                              
                                                                                   
    protected function getColumns(): array                                     
    {                                                                              
        return [                                                                   
            'label' => new Column(
                name: 'Visit_label',
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
        return [
            'candidate' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('CandID')
                ),
                to: schemas\CandidateSchema::class,
                strategy: LoadingStrategy::LAZY,
            ),
            'center' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('CenterID')
                ),
                to: schemas\CenterSchema::class,
                strategy: LoadingStrategy::LAZY,
            ),
        ];
    }                                                                              
}    
