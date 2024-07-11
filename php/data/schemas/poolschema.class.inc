<?php declare(strict_types=1);
/**
 * Pool Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Pool Object instances
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
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;

/**
 * Pool Data Access Object (DAO) Class
 * This class contains all database handling that is needed to
 * permanently store and retrieve Pool Object instances
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

class PoolSchema extends AbstractSchema
{
    protected function initialize(): void
    {
        $this->table = new Table(name: 'biobank_pool');
        $this->id = new Key('PoolId');

        // Define columns
        $this->columns = [
            'label' => new Column(
                name: 'Label', 
                type: ColumnType::VARCHAR
            ),
            'quantity' => new Column(
                name: 'Quantity', 
                type: ColumnType::DECIMAL
            ),
            'date' => new Column(
                name: 'Date', 
                type: ColumnType::DATE
            ),
            'time' => new Column(
                name: 'Time', 
                type: ColumnType::TIME
            ),
        ];

        // Define joins
        $this->joins = [
            'specimen_pool' => new Join(
                from: $this->getIdKey(),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_pool_rel', 
                    ),
                    key: $this->id
                )
            )
        ];
    }

    public static function getRelations(): array
    {
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('UnitID')
                ),
                to: UnitSchema::lazy()
            ),
            'specimen' => new Relation(
                from: new TableKey(
                    table: $this->joins['specimen_pool']->to->table,
                    key: new Key('SpecimenID')
                ),
                to: SpecimenSchema::lazy(),
                many: true
            ),
        ];
    }
}
