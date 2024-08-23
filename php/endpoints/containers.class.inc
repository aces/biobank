<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\endpoints;                                              
                                                                                
use LORIS\biobank\endpoints\container\Types;                                                     
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Service;
use LORIS\biobank\services\ContainerService;                                     
use LORIS\Data\Filters;
                                                                                
final class Containers extends Endpoint
{
    protected function getService(): Service
    {
        return new ContainerService();
    }

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return match ($subpath) {
            'types' => new Types($this->loris),
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
