<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories\specimen\attribute;                                              
                                                                                
use LORIS\biobank\models\specimen\attribute\DataType;                                             
                                                                                
class DataTypeFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return DataType::class;                                                
    }                                                                           
                                                                                
    protected static function mapLazy(string $name, $value)                     
    {                                                                           
        return match ($name) {                                                  
            'id' => $value,                                                     
            'label' => $value,                                                
        };                                                                      
    }                                                                           
                                                                                
    protected static function mapEager(string $name, $value)                    
    {                                                                           
        return self::mapLazy($name, $value);
    }                                                                           
}  
