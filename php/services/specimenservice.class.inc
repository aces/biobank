<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\services;                                                  
                                                                                   
use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;                                                         
use LORIS\biobank\Model;                                                           
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;                                                        
use LORIS\biobank\data\query;                                                        
use LORIS\biobank\data\schemas\SpecimenSchema;                                         
use LORIS\biobank\data\schema\LoadingStrategy;                                         
use LORIS\biobank\models\Specimen;                                                
use LORIS\biobank\validators\SpecimenValidator;                                   
use LORIS\Data\Filters;                                              
                                                                                   
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
        parent::__construct(                                                       
            (new ModelProvisioner(new SpecimenSchema('specimen')))
                ->filter(new Filters\UserSiteMatch())                                  
                ->filter(new Filters\UserProjectMatch()),                                                      
            new SpecimenValidator($this),                                         
        );                                                                         
    }          

    public function applyDefaultParams(query\QueryParams $params): query\QueryParams
    {
        return $params
            ->include(
                new query\With('unit'),
                new query\With('type'),
                new query\With('projects'),
                new query\With('parents'),
                new query\With('candidate'),
                new query\With('session'),
                new query\With('container',
                    subIncludes: [
                        new query\With(name: 'center')
                    ]
                ),
            );
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
