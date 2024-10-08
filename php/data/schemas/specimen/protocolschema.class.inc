<?php                                                                           
                                                                                
namespace LORIS\biobank\data\schemas\specimen;                                  
                                                                                
use \LORIS\biobank\data\Schema;                                                 
use \LORIS\biobank\data\schema\Table;                                           
use \LORIS\biobank\data\schema\TableKey;                                        
use \LORIS\biobank\data\schema\Column;                                          
use \LORIS\biobank\data\schema\ColumnType;                                      
use \LORIS\biobank\data\schema\Join;                                            
use \LORIS\biobank\data\schema\Relation;                                        
use \LORIS\biobank\data\schema\RelationType;                                    
                                                                                
final class ProtocolSchema extends Schema                                       
{                                                                               
    public static function model(): string                                          
    {                                                                           
        return \LORIS\biobank\models\specimen\Protocol::class;                  
    }                                                                           
                                                                                
    public static function table(): Table                                           
    {                                                                           
        return new Table(name: 'biobank_specimen_protocol');                    
    }                                                                           
                                                                                
    public static function id(): string                                             
    {                                                                           
        return 'SpecimenProtocolID';                                            
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
        ];                                                                      
    }                                                                           
                                                                                
    public static function joins(): array                                           
    {                                                                           
        return [                                                                
            'attribute_rel' => new Join(                                        
                from: new TableKey(                                             
                    table: self::table(),                                       
                    key: self::id()                                             
                ),                                                              
                to: new TableKey(                                               
                    table: new Table(                                           
                        name: 'biobank_specimen_protocol_attribute_rel',        
                        alias: 'attribute_rel',                                 
                    ),                                                          
                    key: self::id()                                             
                )                                                               
            )                                                                   
        ];                                                                      
    }                                                                           
                                                                                
    public static function relations(): array                                       
    {                                                                           
        return [                                                                
            'attributes' => new Relation(                                       
                from: new TableKey(                                             
                    table: self::joins()['attribute_rel']->to->table,           
                    key: 'SpecimenAttributeID'                                  
                ),                                                              
                to: AttributeSchema::class,                                     
                many: true,                                                     
            ),                                                                  
            'type' => new Relation(                                             
                from: new TableKey(                                             
                    table: self::table(),                                       
                    key: 'SpecimenTypeID'                                       
                ),                                                              
                to: TypeSchema::class,                                          
            ),                                                                  
            'process' => new Relation(                                          
                from: new TableKey(                                             
                    table: self::table(),                                       
                    key: 'SpecimenProcessID'                                    
                ),                                                              
                to: process\TypeSchema::class,                                  
            ),                                                                  
        ];                                                                      
    }                                                                           
}
