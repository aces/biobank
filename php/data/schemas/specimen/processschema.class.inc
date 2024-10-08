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
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Process::class;                            
    }   

    public static function id(): string                                            
    {                                                                              
        return 'SpecimenID';                                          
    }              

    public static function label(): string                                      
    {                                                                              
        return '';                                                            
    }            

    public static function columns(): array                                     
    {    
        return [
            'date' => new Column(
                name: 'Date',
                table: static::table()
            ),
            'time' => new Column(
                name: 'Time',
                table: static::table()
            ),
            'comments' => new Column(
                name: 'Comments',
                table: static::table()
            ),
            'data' => new Column(
                name: 'Data',
                table: static::table()
            )
        ];
    }

    public static function joins(): array                                     
    {    
        return [];
    }

    public static function relations(): array
    {
        return [
            'protocol' => new Relation(
                from: new TableKey(
                    table: static::table(),
                    key: 'SpecimenProtocolID'
                ),
                to: ProtocolSchema::class,
            ),
            'center' => new Relation(
                from: new TableKey(
                    table: static::table(),
                    key: 'CenterID'
                ),
                to: schemas\CenterSchema::class,
            ),
            'examiner' => new Relation(
                from: new TableKey(
                    table: static::table(),
                    key: 'ExaminerID'
                ),
                to: schemas\ExaminerSchema::class,
            )
        ];
    }
}
