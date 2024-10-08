<?php 

namespace LORIS\biobank\provisioners\specimen;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\specimen\ProtocolSchema;
use \LORIS\biobank\data\query\QueryParams;

final class ProtocolProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new ProtocolSchema('protocol'));
    }

    /**
     * Prepares data or modifies query parameters before loading protocols.
     * 
     * @param QueryParams $queryparams The query parameters for fetching protocol data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a protocol model after it has been loaded, handling relations if necessary.
     *
     * @param Model $protocol The protocol model that has been loaded.
     * @param array $relations An array of related data to be processed with the protocol.
     *
     * @return Model The processed protocol model, currently returned unchanged.
     */
    public function postLoad(Model $protocol, array $relations) : Model
    {
        return $protocol;
    }
}
