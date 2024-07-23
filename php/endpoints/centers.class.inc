<?php declare(strict_types=1);                                                  
                                                                                
namespace LORIS\biobank\Endpoints;                                              
                                                                                
use LORIS\biobank\Endpoint;                                                     
use LORIS\biobank\Controller;                                     
use LORIS\biobank\services\CenterService;
use LORIS\biobank\Factory;
use LORIS\biobank\models\Center;
use LORIS\biobank\Permission;
                                                                                
class Centers extends Endpoint                                                
{                                                                               
    public function __construct(\User $user, \Database $db)                      
    {                                                                           
        $controller = new Controller(
            new CenterService($db),
            new Factory(Center::class)
        );

        parent::__construct($user, $db, $controller);
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

    public static function getName() : string
    {
        return 'Center Endpoint';
    }
}
