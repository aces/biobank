<?php 

namespace LORIS\biobank\provisioners;

use \LORIS\biobank\Model;
use \LORIS\biobank\ModelProvisioner;
use \LORIS\biobank\data\schemas\CenterSchema;
use \LORIS\biobank\data\query\QueryParams;
use \LORIS\biobank\services\SpecimenService;

final class CenterProvisioner extends ModelProvisioner
{
    public function __construct()
    {
        parent::__construct(new CenterSchema('center'));
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
    public function postLoad(Model $center, array $relations) : Model
    {
        return $center;
    }
}
