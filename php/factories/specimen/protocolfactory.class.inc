<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories\specimen;                                              
                                                                                
use LORIS\biobank\models\specimen\Protocol;                                             
use LORIS\biobank\factories\specimen\ProcessFactory;                              
use LORIS\biobank\factories\specimen\TypeFactory;                              
                                                                                
class ProtocolFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Protocol::class;                                                
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
            'type' => TypeFactory::build($value),                                                
            'process' => ProcessFactory::build($value),                                                
        };                                                                      
    }                                                                           
}  
