<?php declare(strict_types=1);

namespace LORIS\biobank\services\shipment;

use LORIS\biobank\provisioners\shipment\StatusProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\shipment\Status;
use LORIS\biobank\validators\shipment\StatusValidator;

/**
 * Service class for handling Status operations.
 *
 * This class initializes the necessary components for Status operations
 * and provides specific implementations for handling post-save actions.
 */
class StatusService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new StatusProvisioner(),
            new Factory(Status::class),
            new StatusValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Status.
     *
     * @param Model $status The saved Status instance.
     */
    public function handlePostSave(Model $status): void {}
}
