<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Validator;
use LORIS\biobank\Model;
use LORIS\biobank\models\Pool;

class PoolValidator extends Validator
{

    protected function getUniqueFields(): array {
        return ['label'];
    }

    protected function getRequiredFields() : array {
        return ['label', '', 'temperature', 'status'];
    }

    protected function getRequiredIfFields(): array {
        return [];
    }

    protected function getFieldTypes() : array {
        return [
            'label' => 'string',
        ];
    }

    public function validateFields(Model $pool) : void
    {
        $this->validateLabel($pool);
        $this->validateSpecimens($pool);
    }

    private function validateLabel(Pool $pool) : void
    {
        $poolList = $this->dao->findByLabel($pool->label);

        if (!empty($poolList)) {
            $this->addError('Pool Label must be unique');
        }
    }


    private function validateSpecimens(Pool $pool) : void
    {
        $specimenIds = $pool->specimenIds;
        if (count($specimenIds) < 2) {
            $this->addError(
                'At least 2 specimens must be provided to create a Pool'
            );
        }

        // XXX: not sure why a loris oobject is being passed to instatiate the
        // DAO. Maybe this is right, but I'm not sure why.
        $specimenDAO   = new SpecimenDAO($this->loris);
        $containerDAO  = new ContainerDAO($this->db);
        $baseSpecimen  = $specimenDAO->getInstanceFromId($specimenIds[0]);
        $baseContainer = $containerDAO->getInstanceFromId(
            $baseSpecimen->containerId
        );

        foreach ($specimenIds as $specimenId) {
            $specimen  = $specimenDAO->getInstanceFromId($specimenId);
            $container = $containerDAO->findById(
                $specimen->containerId
            );
            $barcode   = $container->barcode;

            if ($baseSpecimen->candidateId !== $specimen->candidateId) {
                $this->addError('specimens',
                    'Pooled specimens must belong to the same Candidate'
                );
            }

            if ($baseSpecimen->sessionId !== $specimen->sessionId) {
                $this->addError('specimens',
                    'Pooled specimens must belong to the same Session'
                );
            }

            if ($baseSpecimen->typeId !== $specimen->typeId) {
                $this->addError('specimens', 'Pooled specimens must be of the same Type');
            }

            if ($baseContainer->centerId != $container->centerId) {
                $this->addError('specimens', 'Pooled specimens must be at the same Site');
            }

            if ($specimen->quantity === 0 ) {
                $this->addError('specimens',
                    "Quantity of '$barcode' must be greater than '0'"
                );
            }

            if ($specimen->poolId !== null) {
                $this->addError('specimens',
                    "Specimen '$barcode' already belongs to a Pool"
                );
            }

            $stati    = $containerDAO->stati;
            $statusId = $container->statusId ?? '';
            if ($stati[$statusId]['label'] !== 'Available') {
                $this->addError('specimens', "Specimen '$barcode' is not 'Available'");
            }
        }
    }
}
