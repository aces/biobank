<?php declare(strict_types=1);

namespace LORIS\biobank\services\shipment;

use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\shipment\StatusSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
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
            new ModelProvisioner(new StatusSchema('status')),
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
