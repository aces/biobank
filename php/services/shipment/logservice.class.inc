<?php declare(strict_types=1);

namespace LORIS\biobank\services\shipment;

use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\shipment\LogSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
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
            new ModelProvisioner(new LogSchema('log')),
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
