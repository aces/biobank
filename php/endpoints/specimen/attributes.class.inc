<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints\specimen;                                                 

use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Service;                                                   
use LORIS\biobank\services\specimen\AttributeService;                                       
use LORIS\biobank\endpoints\Permission;                                              
                                                                                   
class Attributes extends Endpoint                                                  
{                                                                                  
    protected function getService(): Service                                       
    {                                                                              
        return new AttributeService();                               
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
}  
