<?php 

namespace LORIS\biobank\provisioners;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\SpecimenSchema;
use \LORIS\biobank\data\query\QueryParams;
use \LORIS\biobank\data\query\Condition;
use \LORIS\biobank\data\query\Operator;

final class SpecimenProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new SpecimenSchema('specimen'));
    }

    /**
     * Prepares data or modifies query parameters before loading specimens.
     * 
     * @param QueryParams $queryparams The query parameters for fetching specimen data.
     * 
     * @return array An array of prepared data, currently returns an empty array.
     */
    protected function preLoad(QueryParams $queryparams): array
    {
        // $query = $this->apply($queryparams->select('containerId'));
        // $query = 'SELECT ContainerID FROM biobank_specimen';
        // $containerIds = $this->db->pselectCol($query, []);
        // //$this->apply($queryparams->select('containerId'))->get());

        // error_log(print_r($containerIds, true));

        // $containerparams = (new QueryParams())
        //     ->where(new Condition(
        //         field: 'id', value: $containerIds, operator: Operator::IN
        //     ));

        // error_log(print_r($containerparams, true));

        // $containers = iterator_to_array((new ContainerProvisioner())->get(), true);
        // $collections = iterator_to_array((new ProcessProvisioner())->get(), true);

        // return [
        //     'containers' => $containers,
        // ];
        return [];
    }

    /**
     * Processes a specimen model after it has been loaded, handling relations if necessary.
     *
     * @param Model $specimen The specimen model that has been loaded.
     * @param array $relations An array of related data to be processed with the specimen.
     *
     * @return Model The processed specimen model, currently returned unchanged.
     */
    public function postLoad(Model $specimen, array $relations) : Model
    {
        $specimen->container = (new ContainerProvisioner())
            ->getById($specimen->container->id);

        return $specimen;
    }
}
