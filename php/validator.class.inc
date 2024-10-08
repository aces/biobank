<?php declare(strict_types=1); 

namespace LORIS\biobank;

/**
 * Abstract class Validator
 *
 * This class provides the core structure for validating models. It
 * manages required fields, field types, and uniqueness constraints,
 * while allowing concrete classes to define specific validation rules.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class Validator
{
    /**
     * Holds validation errors for each model instance.
     *
     * @var array
     */    
    private array $errors = [];
    /**
     * Index of the current model instance being validated.
     *
     * @var int
     */    
    private int $currentIndex;

    /**
     * Constructs the Validator with a service instance.
     *
     * @param Service $service The service associated with this validator.
     */    
    public function __construct(
        protected Service $service
    ) {}

    /**
     * Returns the required fields for validation.
     *
     * @return array An array of required field names.
     */    
    abstract protected function getRequiredFields(): array;

    /**
     * Returns conditionally required fields.
     *
     * @return array An array of field names that are required conditionally.
     */    
    abstract protected function getRequiredIfFields(): array;

    /**
     * Returns the field types for validation.
     *
     * @return array An associative array of field names and their types.
     */
    abstract protected function getFieldTypes(): array;

    /**
     * Returns the fields that must be unique across instances.
     *
     * @return array An array of unique field names.
     */    
    abstract protected function getUniqueFields(): array;

    /**
     * Validates custom fields specific to the model.
     *
     * @param Model $instance The model instance to validate.
     */    
    abstract protected function validateFields(Model $instance) : void;

    /**
     * Adds an error message for a specific field.
     *
     * @param string $field   The field name associated with the error.
     * @param string $message The error message to add.
     */    
    public function addError(string $field, string $message): void
    {
        $index = $this->currentIndex;
        if (!isset($this->errors[$index])) {
            $this->errors[$index] = [];
        }
        if (!isset($this->errors[$index][$field])) {
            $this->errors[$index][$field] = [];
        }
        $this->errors[$index][$field][] = $message;
    }

    /**
     * Checks if any validation errors exist.
     *
     * @return bool True if errors exist, false otherwise.
     */    
    public function hasErrors() : bool
    {
        return !empty($this->errors);
    }

    /**
     * Retrieves all validation errors.
     *
     * @return array An array of errors grouped by field and instance.
     */    
    public function getErrors() : array
    {
        return $this->errors;
    }

    /**
     * Validates a single model instance.
     *
     * The instance is checked for required fields, correct field types,
     * and any custom validations.
     *
     * Note: This method resets the errors array before validation.
     *
     * @param Model $instance The model instance to validate.
     */    
    public function validate(Model $instance) : void 
    {
        $this->errors = [];
        $this->validateRequiredFields($instance);
        $this->validateFieldTypes($instance);
        $this->validateFields($instance);
    }

    /**
     * Validates multiple model instances.
     *
     * Each instance is validated for uniqueness, required fields, and
     * field types. The current index is tracked to associate errors with
     * specific instances.
     *
     * Note: The uniqueness validation assumes that the fields specified
     * in `getUniqueFields()` are always present in each instance.
     *
     * @param array $instances The array of model instances to validate.
     */    
    public function validateInstances(array $instances) : void
    {
        $this->errors = [];
        $this->validateUniqueFields($instances);
        // $this->validateAllFields($instances); // to be implemented

        foreach ($instances as $index => $instance) {
            $this->currentIndex = $index;
            $this->validateRequiredFields($instance);
            // $this->validateRequiredIf($instance); // to be implement
            $this->validateFieldTypes($instance);
            $this->validateFields($instance);
        }
    }

    /**
     * Validates that all required fields are present.
     *
     * Fields are validated based on the list provided by
     * `getRequiredFields()`. Fields must be present and not empty.
     *
     * @param Model $instance The model instance to validate.
     */    
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

    /**
     * Validates that fields have the correct types.
     *
     * Each field is checked to ensure it matches the expected type
     * provided by `getFieldTypes()`.
     *
     * Note: If a field exists but its type does not match, an error is
     * added. There is no type coercion.
     *
     * @param Model $instance The model instance to validate.
     */    
    protected function validateFieldTypes($instance) : void
    {
        foreach ($this->getFieldTypes() as $field => $type) {
            if (isset($instance->$field) && gettype($instance->$field) !== $type) {
                $this->addError($field, "$field must be of type $type.");
            }
        }
    }

    /**
     * Validates that fields marked as unique are unique across instances.
     *
     * Fields specified in `getUniqueFields()` are checked for duplicates
     * across all instances. An error is added if any duplicates are found.
     *
     * @param array $instances The array of model instances to validate.
     */    
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
