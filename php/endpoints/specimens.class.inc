<?php declare(strict_types=1);

namespace LORIS\biobank\endpoints;                                                 
                                                                                   
use LORIS\biobank\endpoints\specimen\Types;
use LORIS\biobank\endpoints\specimen\Protocols;
use LORIS\biobank\endpoints\specimen\Attributes;
use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Service;                                                   
use LORIS\biobank\services\SpecimenService;                                       
use LORIS\Data\Filters;
                                                                                   
class Specimens extends Endpoint                                                  
{                                                                                  
    protected function getService(): Service                                       
    {                                                                              
        return new SpecimenService();
    }      

    protected function getSubHandler(string $subpath): ?Endpoint
    {
        return match ($subpath) {
            'types' => new Types($this->loris),
            'protocols' => new Protocols($this->loris),
            'attributes' => new Attributes($this->loris),
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
}  
