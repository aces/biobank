<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container\type;

use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Key;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\Schema;

/**
 * Dimension Schema Class
 * 
 * This class defines the schema for the `biobank_container_dimension` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class DimensionSchema extends Schema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\container\type\Dimension::class;                          
    }  

    public static function table(): Table {
        return new Table(name: 'biobank_container_dimension');
    }

    public static function id(): string {
        return 'ContainerDimensionID';
    }

    public static function label(): string
    {
        return '';
    }

    public static function columns(): array {
        return [
            'x' => new Column(
                name: 'X',
                table: self::table()
            ),
            'xNum' => new Column(
                name: 'XNumerical',
                table: self::table()
            ),
            'y' => new Column(
                name: 'Y',
                table: self::table()
            ),
            'yNum' => new Column(
                name: 'YNumerical',
                table: self::table()
            ),
            'z' => new Column(
                name: 'Z',
                table: self::table()
            ),
            'zNum' => new Column(
                name: 'ZNumerical',
                table: self::table()
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
