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

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schemas\specimen;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\LoadingStrategy;

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
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen');
    }

    protected function getId(): Key                                            
    {                                                                              
        return new Key('SpecimenID');                                                  
    }        

    protected function getLabel(): string                                      
    {                                                                              
        return 'barcode';                                                            
    }   

    protected function getColumns(): array
    {
        return [
            //TODO may need to change column to 'TableColumn' and include table
            //name or alias so that properties can be pulled from other tables
            //into the main entity (e.g. barcode). -> not sure about it thought
            'quantity' => new Column(name: 'Quantity', type: ColumnType::DECIMAL),
            'sampleNumber' => new Column(name: 'SampleNumber', type: ColumnType::INT),
        ];
    }

    protected function getJoins(): array
    {
        return [
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
                    key: new Key(
                        name: 'ParentSpecimenID'
                    )
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
                    key: new Key(
                        name: 'SessionID',
                    )
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'session',
                        alias: 'session_rel'
                    ),
                    key: new Key(name: 'ID')
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
                    key: new Key('ContainerID')
                ),
                to: ContainerSchema::class,
                strategy: LoadingStrategy::EAGER,
            ),
            'type' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenTypeID')
                ),
                to: specimen\TypeSchema::class,
            ),
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('UnitID')
                ),
                to: UnitSchema::class,
            ),
            'freezethaw' => new Relation(
                from: $this->idKey,
                to: specimen\FreezethawSchema::class,
            ),
            'collection' => new Relation(
                from: $this->idKey,
                to: specimen\process\CollectionSchema::class,
                strategy: LoadingStrategy::EAGER,
            ),
            'preparation' => new Relation(
                from: $this->idKey,
                to: specimen\process\PreparationSchema::class,
                strategy: LoadingStrategy::EAGER,
            ),
            'analysis' => new Relation(
                from: $this->idKey,
                to: specimen\process\AnalysisSchema::class,
                strategy: LoadingStrategy::EAGER,
            ),
            'pool' => new Relation(
                from: new TableKey(
                    table: $this->joins['pool_rel']->to->table,
                    key: new Key('PoolID')
                ),
                to: PoolSchema::class,
                strategy: LoadingStrategy::LAZY,
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
                    key: new Key('ParentSpecimenID')
                ),
                to: self::class,
                many: true
            ),
            'session' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SessionID')
                ),
                to: SessionSchema::class,
            ),
            'candidate' => new Relation(
                from: new TableKey(
                    table: $this->joins['session_rel']->to->table,
                    key: new Key('CandID')
                ),
                to: CandidateSchema::class,
            ),
            'projects' => new Relation(
                from: new TableKey(
                    table: $this->joins['projects_rel']->to->table,
                    key: new Key('ProjectID')
                ),
                to: ProjectSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
        ];
    }
}
