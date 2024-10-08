<?php 

namespace LORIS\biobank\provisioners\shipment;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\shipment\StatusSchema;
use \LORIS\biobank\data\query\QueryParams;

final class StatusProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new StatusSchema('status'));
    }

    /**
     * Prepares data or modifies query parameters before loading statuss.
     * 
     * @param QueryParams $queryparams The query parameters for fetching status data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a status model after it has been loaded, handling relations if necessary.
     *
     * @param Model $status The status model that has been loaded.
     * @param array $relations An array of related data to be processed with the status.
     *
     * @return Model The processed status model, currently returned unchanged.
     */
    public function postLoad(Model $status, array $relations) : Model
    {
        return $status;
    }
}
