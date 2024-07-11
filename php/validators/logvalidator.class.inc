<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Validator;
use LORIS\biobank\models\shipment\Log;

class LogValidator extends Validator
{

    protected function getRequiredFields() : array {
        return ['center', 'status', 'user', 'temperature', 'date','time'];
    }

    protected function getFieldTypes() : array {
        return [
            'center' => 'string',
            '' => 'integer',
            'temperature' =>
            'float',
            'statusId' => 'integer'
        ];
    }

    protected function validateFields(Log $log) : void
    {
    }

}
