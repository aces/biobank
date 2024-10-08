<?php 

namespace LORIS\biobank\provisioners;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\PoolSchema;
use \LORIS\biobank\data\query\QueryParams;

final class PoolProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new PoolSchema('pool'));
    }

    /**
     * Preload the necessary relations based on specimen IDs.
     *
     * @param array $specimenIds The IDs of the specimens whose relations should be preloaded.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Post-process each specimen to attach preloaded relations.
     *
     * @param object $specimen The specimen to modify.
     * @return object Modified specimen with attached relations.
     */
    public function postLoad(Model $pool, array $relations) : Model
    {
        $specimen = (new SpecimenProvisioner())->getById($pool->specimens[0]->id);
        $container = (new ContainerProvisioner())->getById($specimen->container->id);

        $pool->center = $container->center;
        $pool->candidate = $specimen->candidate;
        $pool->session = $specimen->session;
        $pool->projects = $specimen->projects;
        $pool->type = $specimen->type;

        return $pool;
    }
}
