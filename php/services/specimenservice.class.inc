<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\services;                                                  
                                                                                   
use LORIS\biobank\Service;                                                         
use LORIS\biobank\Model;                                                           
use LORIS\biobank\data\DAO;                                                        
use LORIS\biobank\models\Specimen;                                                
use LORIS\biobank\validators\SpecimenValidator;                                   
use LORIS\biobank\schemas\SpecimenSchema;                                         
use LORIS\Data\Filters\UserSiteMatch;                                              
                                                                                   
/**                                                                                
 * Service class for handling Container operations.                                
 *                                                                                 
 * This class initializes the necessary components for Container operations        
 * and provides specific implementations for handling post-save actions.           
 */                                                                                
class SpecimenService extends Service                                             
{                                                                                  
    function __construct()                                                         
    {                                                                              
        $dao = new DAO(\NDB_Factory::singleton()->database(), SpecimenSchema::eager());
        $validator = new SpecimenValidator($dao);                                 
        $dao->filter(new UserSiteMatch());                                         
        parent::__construct($dao, $validator);                                     
    }                                                                              
                                                                                   
    /**                                                                            
     * Handles actions to be performed after saving a Container.                   
     *                                                                             
     * @param Model $instance The saved Container instance.                        
     */                                                                            
    protected function handlePostSave(Model $instance): void                       
    {                                                                              
        if (!$instance instanceof Specimen) {                                     
            throw new \InvalidArgumentException('Expected instance of Container');
        }                                                                          
                                                                                   
        // TODO: Implement post-save logic specific to Container                   
        // Example: Handle parent association changes                              
    }                                                                              
                                                                                   
    //TODO: add a post save here that handles the parent association...            
    // If there is no longer an associated Parent Container ID, delete             
    // it from the entry from the biobank_container_parent table.                  
    // If parent container ID exists, create an association in the                 
    // biobank_container_parent rel table                                          
}    
