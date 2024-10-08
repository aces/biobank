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
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\Type::class;                            
    }   

    public static function table(): Table
    {
        return new Table(name: 'biobank_specimen_type');
    }

    public static function id(): string
    {
        return 'SpecimenTypeID';
    }

    public static function label(): string
    {
        return 'label';
    }

    public static function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Label',
                table: self::table()
            ),
            'freezeThaw' => new Column(
                name: 'FreezeThaw',
                table: self::table()
            ),
        ];
    }

    public static function joins(): array                                       
    {               
        return [
            'type_unit' => new Join(
                from: new TableKey(
                    table: self::table(),
                    key: self::id(),
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_unit_rel', 
                    ),
                    key: self::id()
                )
            ),
            'type_container' => new Join(
                from: new TableKey(
                    table: self::table(),
                    key: self::id(),
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_container_type_rel', 
                    ),
                    key: self::id()
                )
            ),
            'type_parent' => new Join(
                from: new TableKey(
                    table: self::table(),
                    key: self::id(),
                ),
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_type_parent', 
                    ),
                    key: self::id()
                )
            )
        ];
    }

    public static function relations(): array                                   
    { 
        return [
            'units' => new Relation(
                from: new TableKey(
                    table: self::joins()['type_unit']->to->table,
                    key: 'UnitID'
                ),
                to: schemas\UnitSchema::class,
                many: true,
            ),
            'containers' => new Relation(
                from: new TableKey(
                    table: self::joins()['type_container']->to->table,
                    key: 'ContainerTypeID'
                ),
                to: schemas\container\TypeSchema::class,
                many: true,
            ),
            'parents' => new Relation(
                from: new TableKey(
                    table: self::joins()['type_parent']->to->table,
                    key: 'ParentSpecimenTypeID'
                ),
                to: self::class,
                many: true,
            ),
            'protocols' => new Relation(
                from: new TableKey(
                    table: self::table(),
                    key: self::id(),
                ),
                to: ProtocolSchema::class,
                many: true,
            ),
        ];
    }        
}
