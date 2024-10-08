<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\Relation;

/**
 * CandidateSchema Class
 *
 * This class defines the schema for the `candidate` table. It specifies
 * the model, table structure, columns, joins, and relationships with
 * other tables in the biobank system.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @category   Data
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class CandidateSchema extends Schema
{
    public static function model(): string
    {
        return \LORIS\biobank\models\Candidate::class;
    }

    public static function table(): Table
    {
        return new Table(name: 'candidate');
    }

    public static function id(): string
    {
        return 'CandID';
    }

    public static function label(): string
    {
        return 'label';
    }

    public static function columns(): array
    {
        return [
            'label' => new Column(
                name: 'PSCID',
                table: self::table(),
            ),
            'dob' => new Column(
                name: 'DoB',
                table: self::table(),
            ),
            'sex' => new Column(
                name: 'Sex',
                table: self::table(),
            ),
        ];
    }

    public static function joins(): array
    {
        $tableKey = new TableKey(
            table: self::table(),
            key: self::id()
        );
        return [
            'diagnosis' => new Join(
                from: $tableKey,
                to: new TableKey(
                    table: new Table(
                        name: 'candidate_diagnosis_rel'
                    ),
                    key: 'CandID'
                ),
            ),
            'session' => new Join(
                from: $tableKey,
                to: new TableKey(
                    table: new Table(name: 'session'), // to be replaced with call to sesion schema once table name is a static property
                    key: 'CandID'
                )
            ),
        ];
    }

    public static function relations(): array
    {
        return [
            'diagnoses' => new Relation(
                from: new TableKey(
                    table: self::joins()['diagnosis']->to->table,
                    key: 'DiagnosisID'
                ),
                to: DiagnosisSchema::class,
                many: true
            ),
            'sessions' => new Relation(
                from: new TableKey(
                    table: self::joins()['session']->to->table,
                    key: 'ID'
                ),
                to: SessionSchema::class,
                many: true
            ),
        ];
    }
}
