<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;

class TypeSchema extends Schema
{
    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_type');
    }

    protected function getId(): Key
    {
        return new Key('SpecimenTypeID');
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
            'freezeThaw' => new Column(
                name: 'FreezeThaw',
                type: ColumnType::TINYINT
            ),
        ];
    }

    protected function getJoins(): array                                       
    {               
        return [
            'type_unit' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_unit_rel', 
                    ),
                    key: $this->id
                )
            ),
            'type_container' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_container_type_rel', 
                    ),
                    key: $this->id
                )
            ),
            'type_parent' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_parent', 
                    ),
                    key: $this->id
                )
            )
        ];
    }

    protected function getRelations(): array                                   
    { 
        return [
            'unit' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_unit']->to->table,
                    key: new Key('UnitID')
                ),
                to: schemas\UnitSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
            'container' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_container']->to->table,
                    key: new Key('ContainerTypeID')
                ),
                to: schemas\ContainerSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
            'parent' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_parent']->to->table,
                    key: new Key('ParentSpecimenTypeID')
                ),
                to: TypeSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
            'protocols' => new Relation(
                from: $this->idKey,
                to: ProtocolSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
        ];
    }        
}
