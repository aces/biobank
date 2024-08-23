<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

final class UnitSchema extends Schema
{

    protected function getModel(): string
    {
        return \LORIS\biobank\models\Unit::class;
    }

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_unit');
    }

    protected function getId(): string
    {
        return 'UnitID';
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
