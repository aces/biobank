<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\Endpoints;                                              
                                                                                
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Service;                                     
use LORIS\biobank\services\CenterService;
                                                                                
class Centers extends Endpoint                                                
{                                                                               
    protected function getService(): Service                                       
    {                                                                              
        return new CenterService();                             
    }    

    protected static function getPermission(Permission $permission) : string
    {
        return match ($permission) {
            Permission::GET => 'biobank_container_view',
        };
    }

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return null;
    }
}
