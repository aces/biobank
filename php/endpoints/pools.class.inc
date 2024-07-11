<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\Endpoints;                                              
                                                                                
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Controller;                                     
use LORIS\biobank\Permission;
use LORIS\biobank\Factory;
use LORIS\biobank\models\Pool;
use LORIS\biobank\services\PoolService;
                                                                                
class Pools extends Endpoint                                                
{                                                                               
    public function __construct(\User $user)                      
    {                                                                           
        $controller = new Controller(
            new PoolService(),
            new Factory(Pool::class)
        );

        parent::__construct($user, $controller);
    }                                                                           

    protected static function getPermission(Permission $permission) : string
    {
        return match ($permission) {
            Permission::GET => 'biobank_pool_view',
            Permission::POST => 'biobank_pool_create',
        };
    }

    public static function getName() : string
    {
        return 'Pool Endpoint';
    }
}
