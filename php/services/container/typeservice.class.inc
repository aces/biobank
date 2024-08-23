<?php declare(strict_types=1);

namespace LORIS\biobank\services\container;

use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\container\TypeSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\query;
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
            new ModelProvisioner(new TypeSchema('type')),                                                      
            new TypeValidator($this),                                         
        );                                                                         
    }          

    public function applyDefaultParams(query\QueryParams $params): query\QueryParams
    {
        return $params->include(
                new query\With(name: 'capacity'),
                new query\With(name: 'dimension')
            );
    }

    /**
     * Handles actions to be performed after saving a Type.
     *
     * @param Model $type The saved Type instance.
     */
    public function handlePostSave(Model $type): void {}
}
