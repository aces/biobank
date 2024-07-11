<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories\shipment\log;                                              
                                                                                
use LORIS\biobank\models\shipment\log\Status;                                             
                                                                                
class StatusFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Status::class;                                                
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
