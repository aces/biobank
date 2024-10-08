<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\provisioners\ProjectProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\schemas\ProjectSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\models\Project;

/**
 * Service class for handling Project operations.
 *
 * This class initializes the necessary components for Project operations
 * and provides specific implementations for handling post-save actions.
 */
class ProjectService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new ProjectProvisioner(),
            new Factory(Project::class)
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Project.
     *
     * @param Model $project The saved Project instance.
     */
    public function handlePostSave(Model $Project): void {}
}
