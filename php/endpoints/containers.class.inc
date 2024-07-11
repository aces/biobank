<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\endpoints;                                              
                                                                                
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Controller;
use LORIS\biobank\services\ContainerService;                                     
use LORIS\biobank\Factory;                                     
use LORIS\biobank\models\Container;                                     
use LORIS\biobank\Permission;                                     
                                                                                
class Containers extends Endpoint
{
    public function __construct(\User $user, \Database $db)
    {
        $controller = new Controller(
            new ContainerService($db),
            new Factory(Container::class)
        );

        parent::__construct($user, $controller);
    }

    protected static function getPermission(Permission $permission) : string
    {
        return match ($permission) {
            Permission::GET => 'biobank_container_view',
            Permission::PUT => 'biobank_container_update',
            Permission::POST => 'biobank_container_create',
        };
    }

    public static function getName() : string
    {
        return 'Container Endpoint';
    }
}
