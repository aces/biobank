<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\container\type;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\AbstractSchema;

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
final class DimensionSchema extends AbstractSchema
{
    public static function getTable(): Table {
        return new Table(name: 'biobank_container_dimension');
    }

    public static function getId(): Key {
        return new Key(name: 'ContainerDimensionID');
    }

    public static function getLabel(): string
    {
        return '';
    }

    public static function getColumns(): array {
        return [
            'x' => new Column(
                name: 'X',
                type: ColumnType::DECIMAL
            ),
            'xNum' => new Column(
                name: 'XNumerical',
                type: ColumnType::DECIMAL
            ),
            'y' => new Column(
                name: 'Y',
                type: ColumnType::DECIMAL
            ),
            'yNum' => new Column(
                name: 'YNumerical',
                type: ColumnType::DECIMAL
            ),
            'z' => new Column(
                name: 'Z',
                type: ColumnType::DECIMAL
            ),
            'zNum' => new Column(
                name: 'ZNumerical',
                type: ColumnType::DECIMAL
            ),
        ];
    }

    public static function getRelations(): array {
        return []; // Add relation definitions if needed
    }

    public static function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
