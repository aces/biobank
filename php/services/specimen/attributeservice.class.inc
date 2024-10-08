<?php declare(strict_types=1);

namespace LORIS\biobank\services\specimen;

use LORIS\biobank\provisioners\specimen\AttributeProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\specimen\Attribute;
use LORIS\biobank\validators\specimen\AttributeValidator;

/**
 * Service class for handling Attribute operations.
 *
 * This class initializes the necessary components for Attribute operations
 * and provides specific implementations for handling post-save actions.
 */
class AttributeService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new AttributeProvisioner(),
            new Factory(Attribute::class),
            new AttributeValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Attribute.
     *
     * @param Model $attribute The saved Attribute instance.
     */
    public function handlePostSave(Model $type): void {}
}
