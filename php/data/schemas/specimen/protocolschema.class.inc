<?php

namespace LORIS\biobank\data\schemas\specimen;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\LoadingStrategy;

final class ProtocolSchema extends Schema
{
    protected function getTable(): Table                                       
    {                                                                              
        return new Table(name: 'biobank_specimen_protocol');                                    
    }                                                                              
                                                                                   
    protected function getId(): Key                                            
    {                                                                              
        return new Key('SpecimenProtocolID');                                                  
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
                type: ColumnType::VARCHAR
            ),
        ];                                                                         
    }                                                                              
                                                                                   
    protected function getJoins(): array                                       
    {                                                                              
        return [                                                                 
            'attribute_rel' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_protocol_attribute_rel',
                        alias: 'attribute_rel',
                    ),
                    key: $this->id
                )
            )
        ];
    }                                                                              
                                                                                   
    protected function getRelations(): array                                   
    {                                                                              
        return [                                                                 
            'attributes' => new Relation(
                from: new TableKey(
                    table: $this->joins['attribute_rel']->to->table,
                    key: new Key('SpecimenAttributeID')
                ),
                to: AttributeSchema::class,
                strategy: LoadingStrategy::LAZY,
                many: true
            ),
            'type' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenTypeID')
                ),
                to: TypeSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
            'process' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenProcessID')
                ),
                to: process\TypeSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
        ];
    }       
}
