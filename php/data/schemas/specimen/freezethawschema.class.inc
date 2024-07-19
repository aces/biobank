<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

class FreezeThawSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_freezethaw');
    }

    protected function getId(): Key
    {
        return new Key('SpecimenID');
    }

    protected function getLabel(): string
    {
        return 'label';
    }

    protected function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'FreezeThawCycle',
                type: ColumnType::INT
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
