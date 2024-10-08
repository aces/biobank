<?php 

namespace LORIS\biobank\provisioners\shipment;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\shipment\LogSchema;
use \LORIS\biobank\data\query\QueryParams;

final class LogProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new LogSchema('log'));
    }

    /**
     * Prepares data or modifies query parameters before loading logs.
     * 
     * @param QueryParams $queryparams The query parameters for fetching log data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a log model after it has been loaded, handling relations if necessary.
     *
     * @param Model $log The log model that has been loaded.
     * @param array $relations An array of related data to be processed with the log.
     *
     * @return Model The processed log model, currently returned unchanged.
     */
    public function postLoad(Model $log, array $relations) : Model
    {
        return $log;
    }
}
