<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\Schema;


/**
 * Center Schema Class
 * 
 * This class defines the schema for the `psc` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class CenterSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Center::class;                              
    }  

    public static function table(): Table {
        return new Table(name: 'psc');
    }

    public static function id(): string {
        return 'CenterID';
    }

    public static function label(): string {
        return 'label';
    }

    public static function columns(): array {
        return [
            'label' => new Column(
                name: 'Name',
                table: self::table(),
            ),
        ];
    }

    public static function relations(): array {
        return []; // Add relation definitions if needed
    }

    public static function joins(): array {
        return []; // Add join definitions if needed
    }
}
