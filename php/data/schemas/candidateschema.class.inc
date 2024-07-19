<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;

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
class CandidateSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'candidate');
    }

    protected function getId(): Key
    {
        return new Key('CandID');
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'PSCID',
                type: ColumnType::VARCHAR
            ),
            'dob' => new Column(
                name: 'DoB',
                type: ColumnType::DATE
            ),
            'sex' => new Column(
                name: 'Sex',
                type: ColumnType::ENUM,
            ),
        ];
    }

    protected function getJoins(): array
    {
        return [
            'diagnosis' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'candidate_diagnosis_rel'
                    ),
                    key: new Key('CandID')
                ),
            ),
            'session' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(name: 'session'), // to be replaced with call to sesion schema once table name is a static property
                    key: new Key('CandID')
                )
            ),
        ];
    }

    protected function getRelations(): array
    {
        return [
            'diagnoses' => new Relation(
                from: new TableKey(
                    table: $this->joins['diagnosis'],
                    key: new Key(name: 'DiagnosisID')
                ),
                to: DiagnosisSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
            'sessions' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: SessionSchema::getIdKey()->key
                ),
                to: SessionSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
        ];
    }
}
