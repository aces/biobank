<?php

namespace LORIS\biobank\data\schemas\specimen\attribute;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;

class DatatypeSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(
            name: 'biobank_specimen_attribute_datatype', 
        );
        $this->id = new schema\Key('DatatypeID');

        // Define columns
        $this->columns = [
            'datatype' => new schema\Column(
                name: 'Datatype', 
                type: schema\ColumnType::VARCHAR
            )
        ];
    }
}
