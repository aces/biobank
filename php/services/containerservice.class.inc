<?php declare(strict_types=1);

namespace LORIS\biobank\services;


use LORIS\Data\Filters\UserSiteMatch;
use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\query;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\schemas\ContainerSchema;
use LORIS\biobank\models\Container;
use LORIS\biobank\validators\ContainerValidator;

/**
 * Service class for handling Container operations.
 *
 * This class initializes the necessary components for Container operations
 * and provides specific implementations for handling post-save actions.
 */
class ContainerService extends Service
{
    function __construct()
    {
        parent::__construct(
            (new ModelProvisioner(new ContainerSchema('container')))
                ->filter(new UserSiteMatch()),
            new ContainerValidator($this),
        );
    }

    public function applyDefaultParams(query\QueryParams $params): query\QueryParams
    {
        return $params
            ->include(
                new query\With(
                    name: 'type',
                    conditions: [new query\Condition(
                        field: 'prime',
                        value: 0
                    )],
                ),
                new query\With('center'),
                new query\With('parent')
            );
    }

   /**
     * Handles actions to be performed after saving a Container.
     *
     * @param Model $instance The saved Container instance.
     * @throws \InvalidArgumentException If the provided instance is not a Container.
     */
    protected function handlePostSave(Model $instance): void
    {
        if (!$instance instanceof Container) {
            throw new \InvalidArgumentException('Expected instance of Container');
        }

        //TODO: add a post save here that handles the parent association...
        // If there is no longer an associated Parent Container ID, delete
        // it from the entry from the biobank_container_parent table.
        // If parent container ID exists, create an association in the
        // biobank_container_parent rel table
    }
}
