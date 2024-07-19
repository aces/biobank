<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;

abstract class ProcessSchema extends Schema {
    protected function getId(): Key                                            
    {                                                                              
        return new Key('SpecimenID');                                          
    }              

    protected function getLabel(): string                                      
    {                                                                              
        return '';                                                            
    }            

    protected function getColumns(): array                                     
    {    
        return [
            'date' => new Column(
                name: 'Date',
                type: ColumnType::DATE
            ),
            'time' => new Column(
                name: 'Time',
                type: ColumnType::TIME
            ),
            'comments' => new Column(
                name: 'Comments',
                type: ColumnType::VARCHAR
            ),
            'data' => new Column(
                name: 'Data',
                type: ColumnType::LONGTEXT
            )
        ];
    }

    protected function getJoins(): array                                     
    {    
        return [];
    }

    protected function getRelations(): array
    {
        return [
            'protocol' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenProtocolID')
                ),
                to: ProtocolSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('CenterID')
                ),
                to: schemas\CenterSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
            'examiner' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('ExaminerID')
                ),
                to: schemas\ExaminerSchema::class,
                strategy: LoadingStrategy::LAZY
            )
        ];
    }
}
