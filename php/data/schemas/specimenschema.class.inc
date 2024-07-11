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
    protected function initialize(): void {
        // Define table, primary key, label, and table key
        $this->table = new Table(name: 'biobank_specimen');
        $this->id = new Key('SpecimenID');

        // Define columns
        $this->columns = [
            'quantity' => new Column(name: 'Quantity', type: ColumnType::DECIMAL),
            'sampleNumber' => new Column(name: 'SampleNumber', type: ColumnType::INT),
        ];

        // Define joins
        $this->joins = [
            'specimen_pool' => new Join(
                from: $this->getIdKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_pool_rel',
                    ),
                    column: $this->id
                )
            ),
            'specimen_parents' => new Join(
                from: $this->tableKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_parent',
                    ),
                    column: $this->id
                )
            ),
            'specimen_children' => new Join(
                from: $this->tableKey,
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
                from: $this->tableKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_project_rel',
                    ),
                    column: $this->id
                )
            ),
            'session' => new Join(
                from: new TableKey(
                    table: $this->table,
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

        // Define relations
        $this->relations = [
            'container' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('ContainerID')
                ),
                to: ContainerSchema::eager(),
            ),
            'type' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenTypeID')
                ),
                to: specimen\TypeSchema::eager(),
            ),
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('UnitID')
                ),
                to: UnitSchema::eager(),
            ),
            'freezethaw' => new Relation(
                from: $this->tableKey,
                to: specimen\FreezethawSchema::eager(),
            ),
            'collection' => new Relation(
                from: $this->tableKey,
                to: specimen\CollectionSchema::eager(),
            ),
            'preparation' => new Relation(
                from: $this->tableKey,
                to: specimen\PreparationSchema::eager(),
            ),
            'analysis' => new Relation(
                from: $this->tableKey,
                to: Secimen\CollectionSchema::eager(),
            ),
            'pool' => new Relation(
                from: new TableKey(
                    table: $this->joins['specimen_pool'],
                    key: new Key('PoolID')
                ),
                to: PoolSchema::lazy(),
            ),
            'children' => new Relation(
                from: new TableKey(
                    table: $this->joins['specimen_children']->to,
                    key: $this->id
                ),
                to: self::lazy(),
                many: true
            ),
            'parents' => new Relation(
                from: new TableKey(
                    table: $this->joins['specimen_parents']->to,
                    key: new Key('ParentSpecimenID')
                ),
                to: self::lazy(),
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
                    table: $this->joins['specimen_session'],
                    key: new Key('CandidateID')
                ),
                to: CandidateSchema::class,
            ),
            'projects' => new Relation(
                from: new TableKey(
                    table: $this->joins['specimen_projects']->to,
                    key: new Key('ProjectID')
                ),
                to: ProjectSchema::lazy(),
                many: true
            ),
        ];
    }
}
