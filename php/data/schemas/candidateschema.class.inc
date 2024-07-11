<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;

/**
 * Candidate Schema Class
 * 
 * This class defines the schema for the `candidate` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class CandidateSchema extends AbstractSchema
{
    protected function initialize(): void
    {
        // Define table and primary key
        $this->table = new schema\Table(name: 'candidate');
        $this->id = new schema\TableKey(
            table: $this->table,
            key: new schema\Key(name: 'CandID')
        );

        // Define label column
        $this->label = new schema\Column(
            name: 'PSCID',
            type: schema\ColumnType::VARCHAR
        );

        // Define columns
        $this->columns = [
            'pscid' => $this->label,
            'dob' => new schema\Column(
                name: 'DoB',
                type: schema\ColumnType::DATE
            ),
            'sex' => new schema\Column(
                name: 'Sex',
                type: schema\ColumnType::ENUM,
            ),
        ];

        // Define joins
        $this->joins = [
            'diagnosis' => new schema\Join(
                from: $this->getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'candidate_diagnosis_rel'
                    ),
                    key: new schema\Key('CandID')
                ),
            ),
            'session' => new schema\Join(
                from: $this->getIdKey(),
                to: new schema\TableKey(
                    table: SessionSchema::getTable(),
                    key: new schema\Key('CandID')
                )
            ),
        ];

        // Define relations
        $this->relations = [
            'diagnoses' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->join['diagnosis']
                    key: new Key(name: 'DiagnosisID')
                ),
                to: schema\DiagnosisSchema::lazy(),
                many: true
            ),
            'sessions' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table
                    key: schema\SessionSchema::getIdKey()->key
                ),
                to: schema\SessionSchema::lazy(),
                many: true
            ),
        ];
    }
}
