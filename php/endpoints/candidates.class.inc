<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints;                                                 

use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\CandidateService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\Candidate;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Candidates extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new CandidateService($db),                                                
            new Factory(Candidate::class)                                          
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
        return 'Candidate Endpoint';                                               
    }                                                                              
}  
