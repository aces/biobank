<?php

namespace LORIS\biobank\data\schemas\specimen;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\LoadingStrategy;

final class AttributeSchema extends Schema
{
    protected function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Attribute::class;                            
    }   

    protected function table(): Table
    {
        return new Table(name: 'biobank_specimen_attribute');
    }

    protected function id(): string
    {
        return 'SpecimenAttributeID';
    }

    protected function label(): string
    {
        return 'label';
    }

    protected function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Label',
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
        return [
            'datatype' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::table(),
                    key: 'DatatypeID'
                ),
                to: attribute\DatatypeSchema::class,
            )
        ];
    }
}
