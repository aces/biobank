<?php declare(strict_types=1);

namespace LORIS\biobank\services\specimen;

use LORIS\biobank\provisioners\specimen\ProtocolProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\specimen\Protocol;
use LORIS\biobank\validators\specimen\ProtocolValidator;

/**
 * Service class for handling Protocol operations.
 *
 * This class initializes the necessary components for Protocol operations
 * and provides specific implementations for handling post-save actions.
 */
class ProtocolService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new ProtocolProvisioner(),
            new Factory(Protocol::class),
            new ProtocolValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Protocol.
     *
     * @param Model $protocol The saved Protocol instance.
     */
    public function handlePostSave(Model $protocol): void {}
}
