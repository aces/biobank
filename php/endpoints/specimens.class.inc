<?php declare(strict_types=1);

namespace LORIS\biobank\endpoints;                                                 
                                                                                   
use LORIS\biobank\endpoints\specimen\Types;
use LORIS\biobank\endpoints\specimen\Protocols;
use LORIS\biobank\endpoints\specimen\Attributes;
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\SpecimenService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\Specimen;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Specimens extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new SpecimenService($db),                                                
            new Factory(Specimen::class)                                          
        );                                                                         
                                                                                   
        parent::__construct($user, $db, $controller);                                   
    }                                                                              

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return match ($subpath) {
            'types' => new Types($this->user, $this->db),
            'protocols' => new Protocols($this->user, $this->db),
            'attributes' => new Attributes($this->user, $this->db),
            default => null
        };
    }
                                                                                   
    protected static function getPermission(Permission $permission) : string         
    {                                                                              
        return match ($permission) {                                               
            Permission::GET => 'biobank_specimen_view',                            
            Permission::PUT => 'biobank_specimen_update',                            
            Permission::POST => 'biobank_specimen_create',                          
        };                                                                         
    }                                                                              
                                                                                   
    public static function getName() : string                                      
    {                                                                              
        return 'Specimen Endpoint';                                               
    }                                                                              
}  
