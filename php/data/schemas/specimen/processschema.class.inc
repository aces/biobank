<?php

namespace LORIS\biobank\data\schemas\specimen;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;

abstract class ProcessSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Process::class;                            
    }   

    protected function getId(): string                                            
    {                                                                              
        return 'SpecimenID';                                          
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
                table: $this->table,
            ),
            'time' => new Column(
                name: 'Time',
                table: $this->table,
            ),
            'comments' => new Column(
                name: 'Comments',
                table: $this->table,
            ),
            'data' => new Column(
                name: 'Data',
                table: $this->table,
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
                    key: 'SpecimenProtocolID'
                ),
                to: ProtocolSchema::class,
            ),
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'CenterID'
                ),
                to: schemas\CenterSchema::class,
            ),
            'examiner' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'ExaminerID'
                ),
                to: schemas\ExaminerSchema::class,
            )
        ];
    }
}
