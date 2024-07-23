<?php declare(strict_types=1);

namespace LORIS\biobank\services\specimen;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\specimen\AttributeSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
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
    function __construct(\Database $db)
    {
        $dao = new DAO($db,
            new AttributeSchema(
                name: 'attribute',
                strategy: LoadingStrategy::EAGER
            ), new Factory(Attribute::class));
        $validator = new AttributeValidator($dao);
        parent::__construct($dao, $validator);
    }

    /**
     * Handles actions to be performed after saving a Attribute.
     *
     * @param Model $attribute The saved Attribute instance.
     */
    public function handlePostSave(Model $type): void {}
}
