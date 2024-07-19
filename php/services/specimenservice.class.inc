<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\services;                                                  
                                                                                   
use LORIS\biobank\Service;                                                         
use LORIS\biobank\Model;                                                           
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;                                                        
use LORIS\biobank\data\schemas\SpecimenSchema;                                         
use LORIS\biobank\data\schema\LoadingStrategy;                                         
use LORIS\biobank\models\Specimen;                                                
use LORIS\biobank\validators\SpecimenValidator;                                   
use LORIS\Data\Filters\UserSiteMatch;                                              
                                                                                   
/**                                                                                
 * Service class for handling Container operations.                                
 *                                                                                 
 * This class initializes the necessary components for Container operations        
 * and provides specific implementations for handling post-save actions.           
 */                                                                                
class SpecimenService extends Service                                             
{                                                                                  
    function __construct(\Database $db)                                                         
    {                                                                              
        $dao = new DAO($db, new SpecimenSchema(
            name: 'specimen',
            strategy: LoadingStrategy::EAGER
        ), new Factory(Specimen::class));
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
