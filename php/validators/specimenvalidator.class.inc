<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Validator;
use LORIS\biobank\models\Specimen;

class SpecimenValidator extends Validator
{

    protected function getUniqueFields(): array {
        return ['barcode'];
    }

    protected function getRequiredFields() : array {
        return ['type', 'quantity', 'unit', 'candidate', 'session', 'collection'];
    }

    protected function getFieldTypes() : array {
        return [
            'barcode' => 'string',
            'type' => 'string',
            'temperature' => 'float',
            'status' => 'string',
        ];
    }

    private function _validateInstance(Specimen $specimen) : void
        $this->validateTypeId($specimen);
        $this->validateQuantity($specimen);
        $this->validateUnitId($specimen);
        $this->validateFTCycle($specimen);
        $this->validateParentSpecimenIds($specimen);
        $this->validateCollection($specimen);
        $this->validatePreparation($specimen);
        $this->validateAnalysis($specimen);
    }

    private function validateQuantity(Specimen $specimen) : void
    {
        $quantity = $specimen->quantity;
        if (!(is_numeric($quantity) && is_float(floatval($quantity)))) {
            $this->addError('quantity', 'Specimen Quantity value must be a number');
        }
        //TODO: This is where a capacity check would go, i.e. $quantity > $capacity.
        if ($quantity < 0) {
            $this->addError('quantity', 'Specimen Quantity cannot be less than 0');
        }
    }

    private function validateFTCycle(Specimen $specimen) : void
    {
        $fTCycle = $specimen->fTCycle();
        if (isset($fTCycle)) {
            if (strval($fTCycle) !== strval(intval($fTCycle))
                || intval($fTCycle) < 0
            ) {
                $this->addError('fTCycle', "F/T Cycle must be 0 or more");
            }

            if ($fTCycle > 100) {
                $this->addError('ftCycle', "F/T Cycle value is too large");
            }
        }
    }

    /**
     * Validates Specimen Object Parent Specimen IDs.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
                          requirements.
     *
     * @return void
     */
    private function validateParents(Specimen $specimen) : void
    {
        $parents = $specimen->parents;
        if ($parents) {
            $baseParentSpecimen = $this->dao->getInstanceFromId(
                (int) $parents[0]
            );
        }
        $specimenTypes = $this->dao->getTypes();

        if (empty($parents)) {
            return;
        }
        if (empty($baseParentSpecimen)) {
            return;
        }
        foreach ($parents as $parent) {
            $parent = $this->dao->getInstanceFromId((int) $parent);

            $pSpecimenCandId = $parent->getCandidateId();

            if ($baseParentSpecimen->getCandidateId() != $pSpecimenCandId) {
                $this->addError(
                    'parents',
                    'Parent Specimens must be from the same Candidate'
                );
            }

            if ($baseParentSpecimen->getSessionId()!= $parent->getSessionId()
            ) {
                $this->addError(
                    'parents',
                    'Parent Specimens must be from the same Session'
                );
            }

            if ($baseParentSpecimen->getTypeId() != $parent->getTypeId()) {
                $this->addError('parents', 'Parent Specimens must be of the same Type');
            }

            if ($parent->getTypeId() != $specimen->getTypeId()) {
                if (empty($specimenTypes[$specimen->getTypeId()]['parentTypeIds'])) {
                    $parentTypeId = $parent->getTypeId();

                    $childType  = $specimenTypes[$specimen->getTypeId()]['label'];
                    $parentType = $specimenTypes[$parentTypeId]['label'];
                    $this->addError(
                        "A specimen of type '$childType' cannot be derived
                                           from a parent of type '$parentType'"
                    );
                }
                if (!in_array(
                    $parent->getTypeId(),
                    $specimenTypes[$specimen->getTypeId()]['parentTypeIds']
                )
                ) {
                    $this->addError(
                        'Parent Specimens must be of a type that
                                           is a parent to the current Specimen type'
                    );
                }
            }
        }
    }
}
