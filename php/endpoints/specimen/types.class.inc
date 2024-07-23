<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints\specimen;                                                 

use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\specimen\TypeService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\specimen\Type;                                             
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
            Permission::GET => 'biobank_specimen_view',                            
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
