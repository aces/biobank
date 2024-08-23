<?php

namespace LORIS\biobank\data\schemas\specimen\process;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

/**
 * Specimen Process Type Schema Descriptor Class
 * This class encapsulates the table definitions and provides
 * a map to key values to be used within the business layer.
 *
 * PHP Version 7.2
 *
 * @category   Schema
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class TypeSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\process\Type::class;                                
    }   

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_process');
    }

    protected function getId(): string
    {
        return 'SpecimenProcessID';
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'Label',
                table: $this->table,
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
