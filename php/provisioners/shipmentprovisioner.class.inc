<?php 

namespace LORIS\biobank\provisioners;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\ShipmentSchema;
use \LORIS\biobank\data\query\QueryParams;

final class ShipmentProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new ShipmentSchema('shipment'));
    }

    /**
     * Prepares data or modifies query parameters before loading shipments.
     * 
     * @param QueryParams $queryparams The query parameters for fetching shipment data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        return [];
    }

    /**
     * Processes a shipment model after it has been loaded, handling relations if necessary.
     *
     * @param Model $shipment The shipment model that has been loaded.
     * @param array $relations An array of related data to be processed with the shipment.
     *
     * @return Model The processed shipment model, currently returned unchanged.
     */
    public function postLoad(Model $shipment, array $relations) : Model
    {
        return $shipment;
    }
}
