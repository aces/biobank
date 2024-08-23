<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints\shipment;                                                 
                                                                                   
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Service;                                                   
use LORIS\biobank\services\shipment\StatusService;                                       
use LORIS\biobank\endpoints\Permission;                                              
                                                                                   
class Statuses extends Endpoint                                                  
{                                                                                  
    protected function getService(): Service                                       
    {                                                                              
        return new StatusService();                               
    }      

    protected static function getPermission(Permission $permission) : string         
    {                                                                              
        return match ($permission) {                                               
            Permission::GET => 'biobank_container_view',                            
            Permission::PUT => 'biobank_container_update',                            
            Permission::POST => 'biobank_container_create',                          
        };                                                                         
    }                                                                              

    protected function getSubHandler(string $subpath): ?Endpoint                   
    {                                                                              
        return null;
    }     
}  
