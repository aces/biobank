<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;
use LORIS\biobank\data\schemas;

class ProcessSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(name: 'biobank_specimen_collection');
        $this->id = new schema\Key('SpecimenID');

        // Define columns
        $this->columns = [
            'quantity' => new schema\Column(
                name: 'Quantity',
                type: schema\ColumnType::DECIMAL
            ),
            'date' => new schema\Column(
                name: 'Date',
                type: schema\ColumnType::DATE
            ),
            'time' => new Column(
                name: 'Time',
                type: schema\ColumnType::TIME
            ),
            'comments' => new Column(
                name: 'Comments',
                type: schema\ColumnType::VARCHAR
            ),
            'data' => new Column(
                name: 'Data',
                type: schema\ColumnType::LONGTEXT
            )
        ];

        // Define foreign keys
        $this->foreignKeys = [
            'specimenProtocolID' => new schema\TableKey(
                table: $this->table,
                key: new Key('SpecimenProtocolID')
            ),
            'centerID' => new schema\TableKey(
                table: $this->table,
                key: new Key('CenterID')
            ),
            'examinerID' => new schema\TableKey(
                table: $this->table,
                key: new Key('ExaminerID')
            )
        ];

        // Define relations
        $this->relations = [
            'specimenProtocol' => new schema\Relation(
                from: $this->foreignKeys['specimenProtocolID'],
                to: ProtocolSchema::class
            ),
            'center' => new schema\Relation(
                from: $this->foreignKeys['centerID'],
                to: schemas\CenterSchema::class
            ),
            'examiner' => new schema\Relation(
                from: $this->foreignKeys['examinerID'],
                to: schemas\ExaminerSchema::class
            )
        ];
    }
}
