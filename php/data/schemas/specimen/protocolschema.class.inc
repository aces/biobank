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
            'protocol_attribute' => new Join(
                from: $this->idKey,
                to: new TableKey(
                    table: new Table(
                        name: 'biobank_specimen_protocol_attribute_rel',
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
                    table: $this->joins['protocol_attribute']->to->table,
                    key: new Key('SpecimenAttributeID')
                ),
                to: AttributeSchema::class,
                strategy: LoadingStrategy::EAGER,
                many: true
            ),
            'specimenType' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenTypeID')
                ),
                to: TypeSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
            'processType' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key('SpecimenProcessID')
                ),
                to: ProcessSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
        ];
    }       
}
