<?php  

namespace LORIS\biobank;

use LORIS\biobank\data\query\QueryParams;
use \LORIS\Http\Response\JSON\BadRequest;

/**
 * Abstract class Service
 *
 * This class provides core service methods such as retrieving models by ID
 * or label, applying query parameters, and saving models to the database.
 * It includes transaction management and validation for models before
 * persistence.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class Service
{
    /**
     * Constructs the Service with a provisioner, factory, and optional
     * validator.
     *
     * Note: A validator is required when saving instances. If omitted, 
     * saving will throw a LogicException.
     *
     * @param ModelProvisioner  $provisioner The provisioner instance.
     * @param Factory           $factory     The factory for creating models.
     * @param ?Validator        $validator   The validator for model validation.
     */    
    public function __construct(
        protected ModelProvisioner $provisioner,
        protected Factory $factory,
        protected ?Validator $validator = null
    ) {}

    /**
     * Abstract method to handle post-save logic.
     *
     * Child classes should implement this to perform additional actions
     * after saving a model.
     *
     * @param Model $instance The saved model instance.
     */    
    abstract protected function handlePostSave(Model $instance): void;

    /**
     * Retrieves a model by its ID.
     *
     * @param ?int $id The ID of the model to retrieve.
     * @return Model   The retrieved model.
     */    
    public function getById(?int $id): Model
    {
        return $this->provisioner->getById($id);
    }

    /**
     * Retrieves a model by its label.
     *
     * @param ?string $label The label of the model to retrieve.
     * @return Model         The retrieved model.
     */    
    public function getByLabel(?string $label): Model
    {
        return $this->provisioner->getByLabel($label);
    }

    /**
     * Applies query parameters to the provisioner.
     *
     * @param QueryParams $queryparams The query parameters to apply.
     * @return ModelProvisioner        The provisioner instance with the
     *                                 applied parameters.
     */    
    public function apply(QueryParams $queryparams): ModelProvisioner
    {
        return $this->provisioner->apply($queryparams);
    }

    /**
     * Saves one or more models to the database.
     *
     * The instances are validated before saving. If validation fails, the
     * operation is rolled back, and an error response is returned.
     *
     * Note: The validator is required for saving. If it is not provided, a
     * LogicException is thrown.
     *
     * The `handlePostSave` method is called after each model is saved to
     * perform any post-processing logic.
     *
     * @param array ...$instances The instances to be saved.
     * @return array              The response with the status and saved data.
     * @throws \Exception         If an error occurs during the transaction or
     *                            validation.
     */    
    public function save(array ...$instances): array {

        // XXX this is here so that I can omit validators for current services
        // that live in the biobank but shouldn't long term (i.e. centers,
        // projects, candidates, etc.)
        if ($this->validator === null) {
            throw new \LogicException("Validator is required for saving instances.");
        }

        error_log(print_r($instances, true));

        $db = (\NDB_Factory::singleton())->database();
        $persistor = new Persistor($db);
        $persistor->begin();

        try {
            $models = [];
            foreach($instances as $instance) {
                $models[] = $this->factory->create($instance);
            }

            $this->validator->validateInstances($models);

            if ($this->validator->hasErrors()) {
                $persistor->rollback();
                return [
                    'status' => 'error',
                    'errors' => $this->validator->getErrors()
                ];
            }

            $savedInstances = [];
            foreach ($models as $model) {
                $savedInstance = $persistor->save($model);
                $this->handlePostSave($savedInstance);
                $savedInstances[] = $savedInstance;
            }

            $persistor->commit();
            // Return a success response with saved data
            return [
                'status' => 'success',
                'data' => $savedInstances
            ];
        } catch (\Exception $e) {
            $persistor->rollback();
            throw $e;
        }
    }

    /**
     * Deletes a model by its ID.
     *
     * This method begins a transaction, retrieves the model by ID, and
     * deletes it. If the model is not found, an exception is thrown.
     *
     * @param string $id The ID of the model to delete.
     * @throws \Exception If the model is not found or a deletion error
     *                    occurs.
     */    
    public function delete(string $id) : void
    {
        $this->persistor->begin();
        try {
            $instance = $this->provisioner->find($id);
            if (!$instance) {
                throw new Exception("Instance not found");
            }
            $this->persistor->delete($id);
            $this->persistor->commit();
        } catch (Exception $e) {
            $this->persistor->rollback();
            throw $e;
        }
    }
}
