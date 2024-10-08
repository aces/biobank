<?php 

namespace LORIS\biobank\provisioners\shipment;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\shipment\TypeSchema;
use \LORIS\biobank\data\query\QueryParams;

final class TypeProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new TypeSchema('type'));
    }

    /**
     * Prepares data or modifies query parameters before loading types.
     * 
     * @param QueryParams $queryparams The query parameters for fetching type data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a type model after it has been loaded, handling relations if necessary.
     *
     * @param Model $type The type model that has been loaded.
     * @param array $relations An array of related data to be processed with the type.
     *
     * @return Model The processed type model, currently returned unchanged.
     */
    public function postLoad(Model $type, array $relations) : Model
    {
        return $type;
    }
}
