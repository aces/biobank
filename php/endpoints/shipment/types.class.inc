<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints\shipment;                                                 
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\shipment\TypeService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\shipment\Type;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Types extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new TypeService($db),                                                
            new Factory(Type::class)                                          
        );                                                                         
                                                                                   
        parent::__construct($user, $db, $controller);                                   
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
                                                                                   
    public static function getName() : string                                      
    {                                                                              
        return 'Type Endpoint';                                               
    }                                                                              
}  
