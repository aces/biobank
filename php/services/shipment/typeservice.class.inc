<?php declare(strict_types=1);

namespace LORIS\biobank\services\shipment;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\shipment\TypeSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\models\shipment\Type;
use LORIS\biobank\validators\shipment\TypeValidator;

/**
 * Service class for handling Type operations.
 *
 * This class initializes the necessary components for Type operations
 * and provides specific implementations for handling post-save actions.
 */
class TypeService extends Service
{
    function __construct(\Database $db)
    {
        $dao = new DAO($db,
            new TypeSchema(
                name: 'type',
                strategy: LoadingStrategy::EAGER
            ), new Factory(Type::class));
        $validator = new TypeValidator($dao);
        parent::__construct($dao, $validator);
    }

    /**
     * Handles actions to be performed after saving a Type.
     *
     * @param Model $type The saved Type instance.
     */
    public function handlePostSave(Model $type): void {}
}
