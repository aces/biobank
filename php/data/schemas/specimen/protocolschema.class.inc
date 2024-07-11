<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;


class ProtocolSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(name: 'biobank_specimen_protocol');
        $this->id = new schema\Key('SpecimenProtocolID');

        // Define columns
        $this->columns = [
            'label' => new schema\Column(
                name: 'Label',
                type: schema\ColumnType::VARCHAR
            ),
            'specimenProcessID' => new schema\Key('SpecimenProcessID'),
            'specimenTypeID' => new schema\Key('SpecimenTypeID')
        ];

        // Define joins
        $this->joins = [
            'protocol_attribute' => new schema\Join(
                from: $this->tableKey,
                to: new schema\TableKey(
                    table: new schema\Table(
                        name: 'biobank_specimen_protocol_attribute_rel',
                    ),
                    key: $this->id
                )
            )
        ];

        // Define relations
        $this->relations = [
            'attributes' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->joins['protocol_attribute']->to->table,
                    key: new schema\Key('SpecimenAttributeID')
                ),
                to: AttributeSchema::eager(),
                many: true
            ),
            'specimenType' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('SpecimenTypeID')
                ),
                to: TypeSchema::lazy()
            ),
            'processType' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('SpecimenProcessID')
                ),
                to: ProcessSchema::lazy()
            ),
        ];
    }
}
