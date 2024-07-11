<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Validator;
use LORIS\biobank\models\Process;

class ProcessValidator extends Validator
{
    protected function getRequiredFields() : array {
        return ['protocol', 'center', 'examiner'];
    }

    protected function validateFields(Process $process) : void
    {
        $this->validateData($process);
    }

    private function _validateData(Process $process) : void
    {
        $protocolAttributes = $this->dao->getProtocolAttributes();
        $attributeDatatypes = $this->dao->getAttributeDatatypes();
        $protocolId         = $process->getProtocolId();
        $attributes         = $protocolAttributes[$protocolId] ?? [];
        $data = $process->getData() ?? [];

        foreach ($attributes as $id => $attribute) {
            if ($attribute['required'] == true && !isset($data[$id])) {
                $this->addError($wrongAttribute['label'],
                    "The attribute "
                    . $attribute['label']
                    . " is required. Protocol "
                    . $protocolId
                );
            }
        }

        foreach ($data as $attributeId => $value) {
            $attribute = $attributes[$attributeId];

            if (!isset($attribute)) {
                $wrongAttribute = $this->dao->getAttributes()[$attributeId];
                $this->addError($wrongAttribute['label'],
                    "The attribute '"
                    . $wrongAttribute['label']
                    . "' does not belong to this protocol"
                );
            }

            $datatype = $attributeDatatypes[$attribute['datatypeId']]['datatype'];
            if ($datatype === 'boolean' && !is_bool($value)) {
                $this->addError($wrongAttribute['label'],
                    "The attribute '"
                    . $attribute['label']
                    . "' must be a boolean"
                );
            }

            if ($datatype === 'number' && !is_numeric($value)) {
                $this->addError($wrongAttribute['label'],
                    "The attribute '"
                    . $attribute['label']
                    . "' must be a number"
                );
            }

            // TODO: This validation may not be necessary, but it may be good to
            // leave this if statement here just as a reminder that it exists.
            if ($datatype === 'text') {
            }

            // TODO: This can likely be validated through the DateTime Object.
            if ($datatype === 'date') {
            }

            // TODO: This can likely be validated through the DateTime Object.
            if ($datatype === 'time') {
            }

            // TODO: Set this validate when files start to get uploaded.
            if ($datatype === 'file') {
            }
        }
    }
}
