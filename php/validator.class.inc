<?php declare(strict_types=1); 

namespace LORIS\biobank;

use LORIS\biobank\data\DAO;

abstract class Validator
{
    private array $errors = [];
    protected DAO $dao;

    public function __construct(DAO $dao)
    {
        $this->dao = $dao;
    }

    abstract protected function getRequiredFields(): array;
    abstract protected function getRequiredIfFields(): array;
    abstract protected function getFieldTypes(): array;
    abstract protected function getUniqueFields(): array;
    abstract protected function validateFields(Model $instance) : void;

    public function addError(string $field, string $message) : void
    {
        if (!isset($this->errors[$field])) {
            $this->errors[$field] = [];
        }
        $this->errors[$field][] = $message;
    }

    public function hasErrors() : bool
    {
        return !empty($this->errors);
    }

    public function getErrors() : array
    {
        return $this->errors;
    }

    public function validate(Model $instance) : void 
    {
        $this->errors = [];
        $this->validateRequiredFields($instance);
        $this->validateFieldTypes($instance);
        $this->validateFields($instance);
    }

    public function validateInstances(array $instances) : void
    {
        $this->errors = [];
        $this->validateUniqueFields($instances);
        $this->validateAllFields($instances);

        foreach ($instances as $instance) {
            $this->validateRequiredFields($instance);
            $this->validateRequiredIf($instance);
            $this->validateFieldTypes($instance);
            $this->validateFields($instance);
        }
    }

    protected function validateRequiredFields($instance): void
    {
        foreach ($this->getRequiredFields() as $field) {
            if (!isset($instance->$field) ||
                (is_array($instance->$field) 
                && empty($instance->$field))
                || (!is_array($instance->$field)
                && trim($instance->$field) === '')) {
                $this->addError($field, "$field is required.");
            }
        }
    }

    protected function validateFieldTypes($instance) : void
    {
        foreach ($this->getFieldTypes() as $field => $type) {
            if (isset($instance->$field) && gettype($instance->$field) !== $type) {
                $this->addError($field, "$field must be of type $type.");
            }
        }
    }

    protected function validateUniqueFields(array $instances): void
    {
        $uniqueFields = $this->getUniqueFields();
        $fieldValues = [];

        foreach ($instances as $instance) {
            foreach ($uniqueFields as $field) {
                $fieldValues[$field][] = $instance->$field;
            }
        }

        foreach ($fieldValues as $field => $values) {
            if (count($values) !== count(array_unique($values))) {
                $this->addError($field, ucfirst($field) . ' must be unique for each entry.');
            }
        }
    }
}
