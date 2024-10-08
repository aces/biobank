<?php

namespace LORIS\biobank\data\schemas\specimen\attribute;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

final class DatatypeSchema extends Schema
{
    protected function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\attribute\DataType::class;                            
    }   

    protected function table(): Table
    {
        return new Table(name: 'biobank_specimen_attribute_datatype');
    }

    protected function id(): string
    {
        return 'DatatypeID';
    }

    protected function label(): string
    {
        return 'label';
    }

    protected function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Datatype',
                table: self::table()
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
