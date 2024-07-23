<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints;                                                 

use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Server\RequestHandlerInterface;
use \Psr\Http\Message\ResponseInterface;
                                                                                   
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\ShipmentService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\Shipment;                                             
use LORIS\biobank\Permission;                                              
use LORIS\biobank\endpoints\shipment\Types;
use LORIS\biobank\endpoints\shipment\Logs;
use LORIS\biobank\endpoints\shipment\Statuses;
                                                                                   
class Shipments extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new ShipmentService($db),                                                
            new Factory(Shipment::class)                                          
        );                                                                         
                                                                                   
        parent::__construct($user, $db, $controller);                                   
    }                                                                              

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return match ($subpath) {
            'types' => new Types($this->user, $this->db),
            'logs' => new Logs($this->user, $this->db),
            'statuses' => new Statuses($this->user, $this->db),
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
                                                                                   
    public static function getName() : string                                      
    {                                                                              
        return 'Shipment Endpoint';                                               
    }                                                                              
}  
