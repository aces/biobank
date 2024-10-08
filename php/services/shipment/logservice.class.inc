<?php declare(strict_types=1);

namespace LORIS\biobank\services\shipment;

use LORIS\biobank\provisioners\shipment\LogProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\shipment\Log;
use LORIS\biobank\validators\shipment\LogValidator;

/**
 * Service class for handling Log operations.
 *
 * This class initializes the necessary components for Log operations
 * and provides specific implementations for handling post-save actions.
 */
class LogService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new LogProvisioner(),
            new Factory(Log::class),
            new LogValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Log.
     *
     * @param Model $log The saved Log instance.
     */
    public function handlePostSave(Model $Log): void {}
}
