<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

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
class ExaminerSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'examiners');
    }

    protected function getId(): Key
    {
        return new Key('examinerID');
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'full_name',
                type: ColumnType::VARCHAR
            ),
        ];
    }

    protected function getJoins(): array
    {
        return [];
    }

    protected function getRelations(): array
    {
        return [];
    }
}
