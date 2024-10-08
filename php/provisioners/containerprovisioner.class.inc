<?php 

namespace LORIS\biobank\provisioners;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\ContainerSchema;
use \LORIS\biobank\data\query\QueryParams;

final class ContainerProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new ContainerSchema('container'));
    }

    /**
     * Prepares data or modifies query parameters before loading containers.
     * 
     * @param QueryParams $queryparams The query parameters for fetching container data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a container model after it has been loaded, handling relations if necessary.
     *
     * @param Model $container The container model that has been loaded.
     * @param array $relations An array of related data to be processed with the container.
     *
     * @return Model The processed container model, currently returned unchanged.
     */
    public function postLoad(Model $container, array $relations) : Model
    {
        return $container;
    }
}
