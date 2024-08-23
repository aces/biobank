<?php

namespace LORIS\biobank\data\schemas\specimen;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\LoadingStrategy;

final class AttributeSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Attribute::class;                            
    }   

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_attribute');
    }

    protected function getId(): string
    {
        return 'SpecimenAttributeID';
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
        return [
            'datatype' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: 'DatatypeID'
                ),
                to: attribute\DatatypeSchema::class,
            )
        ];
    }
}
