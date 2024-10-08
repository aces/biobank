<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\provisioners\CenterProvisioner;

/**
 * Service class for handling Center operations.
 *
 * This class initializes the necessary components for Center operations
 * and provides specific implementations for handling post-save actions.
 */
class CenterService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new CenterProvisioner(),                                                      
            new \LORIS\biobank\Factory(LORIS\biobank\models\Center::class)
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Center.
     *
     * @param Model $center The saved Center instance.
     */
    public function handlePostSave(Model $Center): void {}
}
