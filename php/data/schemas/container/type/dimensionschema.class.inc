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
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\container\type\Dimension::class;                          
    }  

    protected function getTable(): Table {
        return new Table(name: 'biobank_container_dimension');
    }

    protected function getId(): string {
        return 'ContainerDimensionID';
    }

    protected function getLabel(): string
    {
        return '';
    }

    protected function getColumns(): array {
        return [
            'x' => new Column(
                name: 'X',
                table: $this->table,
            ),
            'xNum' => new Column(
                name: 'XNumerical',
                table: $this->table,
            ),
            'y' => new Column(
                name: 'Y',
                table: $this->table,
            ),
            'yNum' => new Column(
                name: 'YNumerical',
                table: $this->table,
            ),
            'z' => new Column(
                name: 'Z',
                table: $this->table,
            ),
            'zNum' => new Column(
                name: 'ZNumerical',
                table: $this->table,
            ),
        ];
    }

    protected function getRelations(): array {
        return []; // Add relation definitions if needed
    }

    protected function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
