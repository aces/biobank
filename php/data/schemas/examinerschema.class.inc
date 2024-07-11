<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\schema;
use LORIS\biobank\data\AbstractSchema;

/**
 * Examiner Schema Class
 * 
 * This class defines the schema for the `examiners` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class ExaminerSchema extends AbstractSchema
{
    protected function initialize(): void
    {
        // Define table and primary key
        $this->table = new schema\Table(name: 'examiners');
        $this->primaryKey = new schema\TableKey(
            table: $this->table,
            column: new schema\Key('examinerID')
        );

        // Define label column
        $this->label = new schema\Column(
            name: 'full_name',
            type: schema\ColumnType::VARCHAR
        );

        // Define columns
        $this->columns = [
            'label' => $this->label,
        ];
    }
}
