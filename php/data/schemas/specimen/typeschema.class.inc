<?php

namespace LORIS\biobank\data\schemas\specimen;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;

class TypeSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Type::class;                            
    }   

    protected function getTable(): Table
    {
        return new Table(name: 'biobank_specimen_type');
    }

    protected function getId(): string
    {
        return 'SpecimenTypeID';
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
            'freezeThaw' => new Column(
                name: 'FreezeThaw',
                table: $this->table,
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
            'units' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_unit']->to->table,
                    key: 'UnitID'
                ),
                to: schemas\UnitSchema::class,
                many: true,
            ),
            'containers' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_container']->to->table,
                    key: 'ContainerTypeID'
                ),
                to: schemas\container\TypeSchema::class,
                many: true,
            ),
            'parents' => new Relation(
                from: new TableKey(
                    table: $this->joins['type_parent']->to->table,
                    key: 'ParentSpecimenTypeID'
                ),
                to: self::class,
                many: true,
            ),
            'protocols' => new Relation(
                from: $this->idKey,
                to: ProtocolSchema::class,
                many: true,
            ),
        ];
    }        
}
