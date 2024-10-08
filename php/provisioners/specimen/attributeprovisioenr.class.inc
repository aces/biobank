<?php 

namespace LORIS\biobank\provisioners\specimen;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\specimen\AttributeSchema;
use \LORIS\biobank\data\query\QueryParams;

final class AttributeProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new AttributeSchema('attribute'));
    }

    /**
     * Prepares data or modifies query parameters before loading attributes.
     * 
     * @param QueryParams $queryparams The query parameters for fetching attribute data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a attribute model after it has been loaded, handling relations if necessary.
     *
     * @param Model $attribute The attribute model that has been loaded.
     * @param array $relations An array of related data to be processed with the attribute.
     *
     * @return Model The processed attribute model, currently returned unchanged.
     */
    public function postLoad(Model $attribute, array $relations) : Model
    {
        return $attribute;
    }
}
