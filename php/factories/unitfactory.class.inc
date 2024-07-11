<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories;                                              
                                                                                
use LORIS\biobank\models;                                             
                                                                                
class UnitFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Unit::class;                                                
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
