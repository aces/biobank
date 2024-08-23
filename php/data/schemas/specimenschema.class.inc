<?php declare(strict_types=1);
/**
 * Specimen Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Specimen Object instances
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
namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schemas\specimen;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\schema\RelationType;

/**
 * Specimen Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Specimen Object instances
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
class SpecimenSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Specimen::class;                                 
    }  

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen');
    }

    protected function getId(): string                                            
    {                                                                              
        return 'SpecimenID';                                                  
    }        

    protected function getLabel(): string                                      
    {                                                                              
        return 'barcode';                                                            
    }   

    protected function getColumns(): array
    {
        return [
            'quantity' => new Column(
                name: 'Quantity',
                table: $this->table,
            ),
            'freezethaw' => new Column(
                name: 'Quantity',
                table: $this->joins['freezethaw_rel']->to->table,
            ),
            'sampleNumber' => new Column(
                name: 'SampleNumber',
                table: $this->table,
            ),
        ];
    }

    protected function getJoins(): array
    {
        return [
            'freezethaw_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_freezethaw',
                        alias: 'freezethaw_rel',
                    ),
                    key: $this->id
                )
            ),
            'pool_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_pool_rel',
                        alias: 'pool_rel',
                    ),
                    key: $this->id
                )
            ),
            'parents_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_parent',
                        alias: 'parents_rel',
                    ),
                    key: $this->id
                )
            ),
            'children_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_parent',
                        alias: 'children_rel',
                    ),
                    key: 'ParentSpecimenID'
                )
            ),
            'projects_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_project_rel',
                        alias: 'projects_rel',
                    ),
                    key: $this->id
                )
            ),
            'session_rel' => new Join(
                from: new TableKey(
                    table: $this->table,
                    key: 'SessionID'
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'session',
                        alias: 'session_rel'
                    ),
                    key: 'ID'
                )
            ),
        ];
    }

    protected function getRelations(): array                                   
    {                                                                              
        return [
            'container' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'ContainerID'
                ),
                to: ContainerSchema::class,
            ),
            'type' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'SpecimenTypeID'
                ),
                to: specimen\TypeSchema::class,
            ),
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'UnitID'
                ),
                to: UnitSchema::class,
                many: true,
            ),
            'collection' => new Relation(
                from: $this->idKey,
                to: specimen\process\CollectionSchema::class,
            ),
            'preparation' => new Relation(
                from: $this->idKey,
                to: specimen\process\PreparationSchema::class,
            ),
            'analysis' => new Relation(
                from: $this->idKey,
                to: specimen\process\AnalysisSchema::class,
            ),
            'pool' => new Relation(
                from: new TableKey(
                    table: $this->joins['pool_rel']->to->table,
                    key: 'PoolID'
                ),
                to: PoolSchema::class,
            ),
            'children' => new Relation(
                from: new TableKey(
                    table: $this->joins['children_rel']->to->table,
                    key: $this->id
                ),
                to: self::class,
                many: true
            ),
            'parents' => new Relation(
                from: new TableKey(
                    table: $this->joins['parents_rel']->to->table,
                    key: 'ParentSpecimenID'
                ),
                to: self::class,
                many:true,
            ),
            'session' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'SessionID'
                ),
                to: SessionSchema::class,
            ),
            'candidate' => new Relation(
                from: new TableKey(
                    table: $this->joins['session_rel']->to->table,
                    key: 'CandID'
                ),
                to: CandidateSchema::class,
            ),
            'projects' => new Relation(
                from: new TableKey(
                    table: $this->joins['projects_rel']->to->table,
                    key: 'ProjectID'
                ),
                to: ProjectSchema::class,
            ),
        ];
    }
}
