<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema;

/**
 * Diagnosis Schema Class
 * 
 * This class defines the schema for the `diagnosis` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class DiagnosisSchema extends Schema
{
    protected function initialize(): void
    {
        // Define table and primary key
        $this->table = new schema\Table(name: 'diagnosis');
        $this->id = new schema\TableKey(
            table: $this->table,
            column: new schema\Key(name: 'DiagnosisID')
        );

        // Define columns
        $this->columns = [
            'diagnosis' => new Schema\Column(
                name: 'Diagnosis',
                type: schema\ColumnType::VARCHAR
            ),
            'abbreviation' => new schema\Column(
                name: 'Abbreviation',
                type: schema\ColumnType::VARCHAR
            ),
        ];
    }
}
