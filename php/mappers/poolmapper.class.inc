<?php declare(strict_types=1);

namespace LORIS\biobank;

class PoolMapper implements Mapper
{
    private $specimenDAO;

    public function __construct(SpecimenDAO $specimenDAO)
    {
        $this->specimenDAO = $specimenDAO;
    }

    public function map(\User $user, DataInstance $resource): DataInstance
    {
        if ($resource instanceof Pool) {
            $specimens = $resource->getSpecimenBarcodes();
            if (!empty($specimens)) {
                $firstSpecimen = $this->specimenDAO->findById($specimens[0]);
                if ($firstSpecimen) {
                    $resource->setCandidate($firstSpecimen->getCandidate());
                    $resource->setSession($firstSpecimen->getSession());
                    $resource->setType($firstSpecimen->getType());
                    $resource->setCenter($firstSpecimen->getCenter());
                    $resource->setProjects($firstSpecimen->getProjects());
                }
            }
        }
        return $resource;
    }
}
