<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints;                                                 

use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Service;                                                   
use LORIS\biobank\services\ShipmentService;                                       
use LORIS\biobank\endpoints\shipment\Types;
use LORIS\biobank\endpoints\shipment\Logs;
use LORIS\biobank\endpoints\shipment\Statuses;
                                                                                   
class Shipments extends Endpoint                                                  
{                                                                                  
    protected function getService(): Service                                       
    {                                                                              
        return new ShipmentService();                               
    }      

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return match ($subpath) {
            'types' => new Types($this->loris),
            'logs' => new Logs($this->loris),
            'statuses' => new Statuses($this->loris),
            default => null
        };
    }

    protected static function getPermission(Permission $permission) : string         
    {                                                                              
        return match ($permission) {                                               
            Permission::GET => 'biobank_container_view',                            
            Permission::PUT => 'biobank_container_update',                            
            Permission::POST => 'biobank_container_create',                          
        };                                                                         
    }                                                                              
}  
