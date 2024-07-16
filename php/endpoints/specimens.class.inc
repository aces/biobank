<?php declare(strict_types=1);

namespace LORIS\biobank\endpoints;                                                 
                                                                                   
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
                                                                                   
        parent::__construct($user, $controller);                                   
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
