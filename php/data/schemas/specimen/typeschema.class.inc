<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schema;

class TypeSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(
            name: 'biobank_specimen_type',
        );
        $this->primary = new schema\Key('SpecimenTypeID');

        // Define columns
        $this->columns = [
            'label' => new schema\Column(
                name: 'Label',
                type: schema\ColumnType::VARCHAR
            ),
            'freezeThaw' => new schema\Column(
                name: 'FreezeThaw',
                type: schema\ColumnType::TINYINT
            ),
        ];

        // Define joins
        $this->joins = [
            'type_unit' => new Join(
                from: $this->getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_specimen_type_unit_rel', 
                    ),
                    key: $this->primary
                )
            ),
            'type_container' => new schema\Join(
                from: $this->getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_specimen_type_container_type_rel', 
                    ),
                    key: $this->primary
                )
            ),
            'type_parent' => new schema\Join(
                from: $this->getIdKey(),
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_specimen_type_parent', 
                    ),
                    key: $this->primary
                )
            )
        ];

        // Define relations
        $this->relations = [
            'unit' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->joins['type_unit']->to->table,
                    key: new schema\Key('UnitID')
                ),
                to: schemas\UnitSchema::lazy(),
                many: true
            ),
            'container' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->joins['type_container']->to->table,
                    key: new schema\Key('ContainerTypeID')
                ),
                to: schemas\ContainerSchema::lazy(),
                many: true
            ),
            'parent' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->joins['type_parent']->to->table,
                    key: new schema\Key('ParentSpecimenTypeID')
                ),
                to: TypeSchema::lazy(),
                many: true
            ),
            'protocols' => new schema\Relation(
                from: $this->getIdKey(),
                to: ProtocolSchema::lazy(),
                many: true
            ),
        ];
    }        
}
