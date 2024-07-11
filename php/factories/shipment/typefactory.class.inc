<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories\shipment;                                              
                                                                                
use LORIS\biobank\models\shipment\Type;                                             
                                                                                
class TypeFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Type::class;                                                
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
        return match ($name) {                                                  
            'id' => $value,                                                     
            'label' => $value,                                                
        };                                                                      
    }                                                                           
}   
