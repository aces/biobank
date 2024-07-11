<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories;                                              
                                                                                
use LORIS\biobank\models\Shipment;                                             
use LORIS\biobank\factories\shipment\TypeFactory;                                    
use LORIS\biobank\factories\CenterFactory;                                      
use LORIS\biobank\factories\ContainersFactory;                                  
use LORIS\biobank\factories\shipment\LogFactory;                                  
use \DateTime;                                                                  
                                                                                
class ShipmentFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Shipment::class;                                                
    }                                                                           
                                                                                
    protected static function mapLazy(string $name, $value)                     
    {                                                                           
        return match ($name) {                                                  
            'id' => $value,                                                     
            'barcode' => $value,                                                
        };                                                                      
    }                                                                           
                                                                                
    protected static function mapEager(string $name, $value)                    
    {                                                                           
        return match ($name) {                                                  
            'id' => $value,                                                     
            'barcode' => $value,                                                
            'type' => TypeFactory::build($value),                          
            'containers' => ContainerFactory::buildMany($value), // Lazy creation for related models
            'logs' => LogFactory::buildManyEager($value),                                     
        };                                                                      
    }                                                                           
}   
