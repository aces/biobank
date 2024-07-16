<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints;                                                 
                                                                                   
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\ShipmentService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\Shipment;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Shipments extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new ShipmentService($db),                                                
            new Factory(Shipment::class)                                          
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
        return 'Shipment Endpoint';                                               
    }                                                                              
}  
