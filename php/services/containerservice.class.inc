<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\query\Condition;
use LORIS\biobank\data\query\QueryParams;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\schemas\ContainerSchema;
use LORIS\biobank\data\schemas\container\TypeSchema;
use LORIS\biobank\models\Container;
use LORIS\biobank\validators\ContainerValidator;
use LORIS\Data\Filters\UserSiteMatch;

/**
 * Service class for handling Container operations.
 *
 * This class initializes the necessary components for Container operations
 * and provides specific implementations for handling post-save actions.
 */
class ContainerService extends Service
{
    function __construct(\Database $db)
    {
        $dao = new DAO(
            $db,
            new ContainerSchema(
                name: 'container',
                strategy: LoadingStrategy::EAGER,
            ),
            new Factory(Container::class)
        );
        $validator = new ContainerValidator($dao);
        $dao->filter(new UserSiteMatch());
        parent::__construct($dao, $validator);
    }

    /**
     * Finds containers based on the provided query parameters, always including only primary type containers.
     *
     * @param QueryParams $queryParams The query parameters to use for finding containers.
     * @return array An array of Model instances representing the found containers.
     */
    public function getInstances(QueryParams $queryParams): array
    {
        // Always add the 'primary' condition
        // TODO: I'm convinced there's a better of doign this.
        $queryParams->conditions[] = new Condition(
            field: 'prime',
            value: 0,
            relation: new TypeSchema(name: 'container_type', strategy: LoadingStrategy::LAZY),
        );

        return parent::getInstances($queryParams);
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

        // TODO: Implement post-save logic specific to Container
        // Example: Handle parent association changes
    }

    //TODO: add a post save here that handles the parent association...
    // If there is no longer an associated Parent Container ID, delete
    // it from the entry from the biobank_container_parent table.
    // If parent container ID exists, create an association in the
    // biobank_container_parent rel table
}
