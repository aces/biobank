<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\Endpoints;                                              
                                                                                
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Service;                                     
use LORIS\biobank\services\PoolService;
use LORIS\Data\Filters;
                                                                                
final class Pools extends Endpoint                                                
{                                                                               
    protected function getService(): Service                                       
    {                                                                              
        return new PoolService();                              
    }    

    protected static function getPermission(Permission $permission) : string
    {
        return match ($permission) {
            Permission::GET => 'biobank_pool_view',
            Permission::POST => 'biobank_pool_create',
        };
    }

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return null;
    }
}
