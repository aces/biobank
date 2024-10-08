<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

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
    protected function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Examiner::class;                              
    }  

    protected function table(): Table
    {
        return new Table(name: 'examiners');
    }

    protected function id(): string
    {
        return 'examinerID';
    }

    protected function label(): string
    {
        return 'label';
    }

    protected function columns(): array
    {
        return [
            'label' => new Column(
                name: 'full_name',
                table: self::table(),
            ),
        ];
    }

    protected function joins(): array
    {
        return [];
    }

    protected function relations(): array
    {
        return [];
    }
}
