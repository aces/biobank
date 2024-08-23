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
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\Center::class;                              
    }  

    protected function getTable(): Table {
        return new Table(name: 'psc');
    }

    protected function getId(): string {
        return 'CenterID';
    }

    protected function getLabel(): string {
        return 'label';
    }

    protected function getColumns(): array {
        return [
            'label' => new Column(
                name: 'Name',
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
