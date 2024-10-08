<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

/**
 * Project Schema Class
 * 
 * This class defines the schema for the `projects` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class ProjectSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Project::class;                              
    }  

    public static function table(): Table
    {
        return new Table(name: 'Project');
    }

    public static function id(): string
    {
        return 'ProjectID';
    }

    public static function label(): string
    {
        return 'label';
    }

    public static function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Alias',
                table: self::table()
            ),
        ];
    }

    public static function joins(): array
    {
        return [];
    }

    public static function relations(): array
    {
        return [];
    }
}
