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

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schemas\specimen;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\Join;

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
class SpecimenSchema extends AbstractSchema {
    public static function getTable(): Table
    {
        return new Table(name: 'biobank_specimen');
    }

    public static function getId(): Key                                            
    {                                                                              
        return new Key('SpecimenID');                                                  
    }        

    public static function getLabel(): string                                      
    {                                                                              
        return 'barcode';                                                            
    }   

    public static function getColumns(): array
    {
        return [
            'quantity' => new Column(name: 'Quantity', type: ColumnType::DECIMAL),
            'sampleNumber' => new Column(name: 'SampleNumber', type: ColumnType::INT),
        ];
    }

    public static function getJoins(): array
    {
        return [
            'specimen_pool' => new Join(
                from: self::getIdKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_pool_rel',
                    ),
                    column: self::getId
                )
            ),
            'specimen_parents' => new Join(
                from: self::getIdKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_parent',
                    ),
                    column: self::getId
                )
            ),
            'specimen_children' => new Join(
                from: self::getIdKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_parent',
                    ),
                    column: new Column(
                        name: 'ParentSpecimenID',
                        type: ColumnType::INT
                    )
                )
            ),
            'specimen_projects' => new Join(
                from: self::getTableKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_project_rel',
                    ),
                    column: self::getId()
                )
            ),
            'session' => new Join(
                from: new TableKey(
                    table: self::getTable(),
                    column: new Column(
                        name: 'SessionID',
                        type: ColumnType::INT
                    )
                ),
                to: new TableKey(
                    table: new Table(name: 'session'),
                    column: new Column(name: 'ID', type: ColumnType::INT)
                )
            ),
        ];
    }

    public static function getRelations(): array                                   
    {                                                                              
        return [
            'container' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key('ContainerID')
                ),
                to: ContainerSchema::eager(),
            ),
            'type' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key('SpecimenTypeID')
                ),
                to: specimen\TypeSchema::eager(),
            ),
            'unit' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key('UnitID')
                ),
                to: UnitSchema::eager(),
            ),
            'freezethaw' => new Relation(
                from: self::getIdKey(),
                to: specimen\FreezethawSchema::eager(),
            ),
            'collection' => new Relation(
                from: self::getIdKey(),
                to: specimen\CollectionSchema::eager(),
            ),
            'preparation' => new Relation(
                from: self::getIdKey(),
                to: specimen\PreparationSchema::eager(),
            ),
            'analysis' => new Relation(
                from: self::getIdKey(),
                to: Secimen\CollectionSchema::eager(),
            ),
            'pool' => new Relation(
                from: new TableKey(
                    table: self::getJoins()['specimen_pool'],
                    key: new Key('PoolID')
                ),
                to: PoolSchema::lazy(),
            ),
            'children' => new Relation(
                from: new TableKey(
                    table: self::getJoins()['specimen_children']->to->table,
                    key: self::getId()
                ),
                to: self::lazy(),
                many: true
            ),
            'parents' => new Relation(
                from: new TableKey(
                    table: self::getJoin()['specimen_parents']->to->table,
                    key: new Key('ParentSpecimenID')
                ),
                to: self::lazy(),
                many: true
            ),
            'session' => new Relation(
                from: new TableKey(
                    table: self::getTable(),
                    key: new Key('SessionID')
                ),
                to: SessionSchema::lazy(),
            ),
            'candidate' => new Relation(
                from: new TableKey(
                    table: self::getJoins()['specimen_session'],
                    key: new Key('CandidateID')
                ),
                to: CandidateSchema::lazy(),
            ),
            'projects' => new Relation(
                from: new TableKey(
                    table: self::getJoins()['specimen_projects']->to->table,
                    key: new Key('ProjectID')
                ),
                to: ProjectSchema::lazy(),
                many: true
            ),
        ];
    }
}
