<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\factories;                                              
                                                                                
use LORIS\biobank\models\shipment\Log;                                             
use LORIS\biobank\factories\ShipmentFactory;                                    
use LORIS\biobank\factories\CenterFactory;                                      
use LORIS\biobank\factories\StatusFactory;                                      
use LORIS\biobank\factories\UserFactory;                                      
use \DateTime;                                                                  
                                                                                
class LogFactory extends ModelFactory                                     
{                                                                               
    protected static function getModelClass(): string {                         
        return Log::class;                                                
    }                                                                           
                                                                                
    protected static function mapLazy(string $name, $value)                     
    {                                                                           
        return match ($name) {                                                  
            'shipment' => ShipmentFactory::build($value),                                                     
            'center' => CenterFactory::build($value),                                                
            'status' => StatusFactory::buildEager($value),                                                
            'user' => UserFactory::build($value),                                                
            'temperature' => $value,
            'date' => new DateTime($value),
            'time' => new DateTime($value),
            'comments' => $value,
        };                                                                      
    }                                                                           
                                                                                
    protected static function mapEager(string $name, $value)                    
    {                                                                           
        return match ($name) {                                                  
            'shipment' => ShipmentFactory::build($value),                                                     
            'center' => CenterFactory::build($value),                                                
            'status' => StatusFactory::buildEager($value),                                                
            'user' => UserFactory::build($value),                                                
            'temperature' => $value,
            'date' => new DateTime($value),
            'time' => new DateTime($value),
            'comments' => $value,
        };                                                                      
    }                                                                           
}   
