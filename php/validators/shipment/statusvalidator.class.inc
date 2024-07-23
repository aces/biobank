<?php declare(strict_types=1); 

namespace LORIS\biobank\validators\shipment;

use LORIS\biobank\Model;
use LORIS\biobank\Validator;

class StatusValidator extends Validator
{

    protected function getUniqueFields(): array {
        return [];
    }

    protected function getRequiredFields() : array {
        return [];
    }

    protected function getFieldTypes() : array {
        return [];
    }

    protected function getRequiredIfFields(): array
    {
        return [];
    }

    protected function validateFields(Model $log) : void
    {
    }
}
