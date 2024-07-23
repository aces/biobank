<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\LoadingStrategy;

final class AttributeSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_attribute');
    }

    protected function getId(): Key
    {
        return new Key('SpecimenAttributeID');
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
                type: ColumnType::VARCHAR
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
                    key: new Key('DatatypeID')
                ),
                to: attribute\DatatypeSchema::class,
                strategy: LoadingStrategy::EAGER
            )
        ];
    }
}
