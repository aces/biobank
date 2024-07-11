<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\schema;
use LORIS\biobank\data\AbstractSchema;

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
class SessionSchema extends AbstractSchema
{
    protected function initialize(): void
    {
        // Define table and primary key
        $this->table = new schema\Table(name: 'session');
        $this->id = new schema\TableKey(
            table: $this->table,
            column: new schema\Key(name: 'ID')
        );

        // Define label column
        $this->label = new schema\Column(
            name: 'Visit_label',
            type: schema\ColumnType::VARCHAR
        );

        // Define columns
        $this->columns = [
            'visit_label' => $this->label,
        ];

        // Define relations
        $this->relations = [
            'candidate' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('CandID')
                ),
                to: schema\CandidateSchema::lazy()
            ),
            'center' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('CenterID')
                ),
                to: schema\CenterSchema::lazy()
            ),
        ];
    }
}
