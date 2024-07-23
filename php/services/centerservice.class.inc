<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\CenterSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\models\Center;

/**
 * Service class for handling Center operations.
 *
 * This class initializes the necessary components for Center operations
 * and provides specific implementations for handling post-save actions.
 */
class CenterService extends Service
{
    function __construct(\Database $db)
    {
        $dao = new DAO($db,
            new CenterSchema(
                name: 'center',
                strategy: LoadingStrategy::EAGER
            ), new Factory(Center::class));
        parent::__construct($dao);
    }

    /**
     * Handles actions to be performed after saving a Center.
     *
     * @param Model $center The saved Center instance.
     */
    public function handlePostSave(Model $Center): void {}
}
