<?php

namespace LORIS\biobank\data\schemas\specimen\attribute;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

final class DatatypeSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_attribute_datatype');
    }

    protected function getId(): Key
    {
        return new Key('DatatypeID');
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'Datatype',
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
        return [];
    }
}
