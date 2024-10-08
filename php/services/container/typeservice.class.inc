<?php declare(strict_types=1);

namespace LORIS\biobank\services\container;

use LORIS\biobank\provisioners\container\TypeProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\container\Type;
use LORIS\biobank\validators\container\TypeValidator;

/**
 * Service class for handling Type operations.
 *
 * This class initializes the necessary components for Type operations
 * and provides specific implementations for handling post-save actions.
 */
class TypeService extends Service
{
    public function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new TypeProvisioner(),                                                      
            new Factory(Type::class),
            new TypeValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Type.
     *
     * @param Model $type The saved Type instance.
     */
    public function handlePostSave(Model $type): void {}
}
