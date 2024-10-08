<?php declare(strict_types=1);

namespace LORIS\biobank\services;


use LORIS\Data\Filters\UserSiteMatch;
use LORIS\biobank\Model;
use LORIS\biobank\Service;
use LORIS\biobank\Factory;
use LORIS\biobank\provisioners\ContainerProvisioner;
use LORIS\biobank\validators\ContainerValidator;
use LORIS\biobank\models\Container;

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
            (new ContainerProvisioner())
                ->filter(new UserSiteMatch()),
            new Factory(Container::class),
            new ContainerValidator($this),
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
        //TODO: add a post save here that handles the parent association...
        // If there is no longer an associated Parent Container ID, delete
        // it from the entry from the biobank_container_parent table.
        // If parent container ID exists, create an association in the
        // biobank_container_parent rel table
    }
}
