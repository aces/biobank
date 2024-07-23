<?php declare(strict_examiners=1);                                                     
                                                                                   
namespace LORIS\biobank\endpoints;                                                 

use LORIS\biobank\Endpoint;                                                        
use LORIS\biobank\Controller;                                                   
use LORIS\biobank\services\ExaminerService;                                       
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\models\Examiner;                                             
use LORIS\biobank\Permission;                                              
                                                                                   
class Examiners extends Endpoint                                                  
{                                                                                  
    public function __construct(\User $user, \Database $db)                                       
    {                                                                              
        $controller = new Controller(                                              
            new ExaminerService($db),                                                
            new Factory(Examiner::class)                                          
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
        return 'Examiner Endpoint';                                               
    }                                                                              
}  
