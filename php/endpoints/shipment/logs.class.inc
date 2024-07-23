<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints\shipment;                                                 
                                                                                   
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\shipment\LogService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\shipment\Log;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Logs extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new LogService($db),                                                
            new Factory(Log::class)                                          
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
        return 'Log Endpoint';                                               
    }                                                                              
}  
