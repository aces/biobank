<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\services;                                                  
                                                                                   
use LORIS\biobank\provisioners\SpecimenProvisioner;
use LORIS\biobank\Service;                                                         
use LORIS\biobank\Model;                                                           
use LORIS\biobank\Factory;
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
            (new SpecimenProvisioner())
                ->filter(new Filters\UserSiteMatch())
                ->filter(new Filters\UserProjectMatch()),
            new Factory(Specimen::class),
            new SpecimenValidator($this),                                         
        );                                                                         
    }          

                                                                                   
    /**                                                                            
     * Handles actions to be performed after saving a Container.                   
     *                                                                             
     * @param Model $instance The saved Container instance.                        
     */                                                                            
    protected function handlePostSave(Model $specimen): void                       
    {                                                                              
        $specimen->collection->id = $specimen->id;
        $specimen->preparation->id = $specimen->id;
        $specimen->analysis->id = $specimen->id;

        (new ContainerService)->save($specimen->container);
        (new ProcessService)
            ->save($specimen->collection)
            ->save($specimen->preparation)
            ->save($specimen->analysis);
    }                                                                              
}    
