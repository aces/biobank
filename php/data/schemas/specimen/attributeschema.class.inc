<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;
use LORIS\biobank\data\schemas\specimen\attribute;

class SpecimenAttributeSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(name: 'biobank_specimen_attribute');
        $this->id = new schema\Key('SpecimenAttributeID');

        // Define columns
        $this->columns = [
            'label' => new schema\Column(
                name: 'Label',
                type: schema\ColumnType::VARCHAR
            ),
            'datatypeID' => new Key('DatatypeID')
        ];

        // Define relations
        $this->relations = [
            'datatype' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new Key('DatatypeID')
                ),
                to: attribute\DatatypeSchema::class
            )
        ];
    }
}
