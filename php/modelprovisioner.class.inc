<?php

namespace LORIS\biobank;

use \LORIS\biobank\data\query\Query;
use \LORIS\biobank\data\query\With;
use \LORIS\biobank\data\query\Condition;
use \LORIS\biobank\data\query\QueryParams;
use \LORIS\biobank\data\query\QueryBuilder;
use \LORIS\biobank\data\Schema;

/**
 * Abstract class ModelProvisioner
 *
 * Provides methods to manage the retrieval and processing of models
 * based on database queries. Handles preloading, post-processing, and
 * the fetching of model instances with customizable query parameters.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
abstract class ModelProvisioner extends \LORIS\Data\ProvisionerInstance
{
    /**
     * Stores relationships to be loaded with the models.
     *
     * @var array
     */    
    private array $withs = [];

    /**
     * Stores the query parameters to be applied when fetching models.
     *
     * @var QueryParams|null
     */
    private ?QueryParams $queryparams = null;

    /**
     * Constructs the ModelProvisioner with a schema.
     *
     * @param Schema $schema The schema that defines the returned objects.
     */    
    public function __construct(
        private Schema $schema
    ) {
        $this->db = (\NDB_Factory::singleton())->database();
    }

    /**
     * Pre-loads data for the query based on query parameters.
     *
     * @param QueryParams $queryparams The query parameters to apply.
     * @return array                   Preloaded data.
     */    
    abstract protected function preLoad(QueryParams $queryparams): array;

    /**
     * Post-processes the loaded model instance after preloading.
     *
     * @param Model $specimen  The model to post-process.
     * @param array $preload   Preloaded data for further processing.
     * @return Model           The post-processed model.
     */    
    abstract protected function postLoad(Model $specimen, array $preload): Model;

    /**
     * Applies query parameters to this provisioner.
     *
     * @param QueryParams|null $queryparams The query parameters to apply.
     * @return self                         The current instance.
     */    
    public function apply(?QueryParams $queryparams): self
    {
        $this->queryparams = $queryparams;
        return $this;
    }

    /**
     * Retrieves a model by its ID.
     *
     * @param int|null $id The ID of the model to retrieve.
     * @return Model       The retrieved model instance.
     */    
    public function getById(?int $id): Model
    {
        $params = (new QueryParams())
            ->where(new Condition(field: 'id', value: $id));

        return $this->apply($params)->getInstance();
    }

    /**
     * Retrieves a model by its label.
     *
     * @param string|null $label The label of the model to retrieve.
     * @return Model             The retrieved model instance.
     */    
    public function getByLabel(?string $label): Model
    {
        $params = (new QueryParams())
            ->where(new Condition(field: $this->schema::label(), value: $label));
        return $this->apply($params)->getInstance();
    }

    /**
     * Retrieves a single instance of the model.
     *
     * @return Model|null The first retrieved model instance, or null.
     */    
    public function getInstance(): ?Model
    {
        $generator = $this->get();
        foreach ($generator as $modelId => $model) {
            return $model; // Return the first (and only) model
        }
        return null;
    }

    /**
     * Specifies relationships to be loaded with the models.
     *
     * @param With ...$relations The relationships to load.
     * @return self              The current instance.
     */    
    public function with(With ...$relations): self
    {
        // Append new relations to the existing $withs array
        $this->withs = array_merge($this->withs, $relations);
        return $this;
    }

    /**
     * Retrieves models without additional processing.
     *
     * @return \Traversable The models as a traversable object.
     */    
    public function get(): \Traversable
    {
        return $this->fetchModels(function($model) {
            return $model;
        });
    }
    
    /**
     * Retrieves all instances with preloading and post-processing.
     *
     * @return \Traversable The processed models as a traversable object.
     */    
    public function getAllInstances(): \Traversable
    {
        $queryparams = $this->queryparams ?? new QueryParams();
        $preload = $this->preLoad($queryparams);
    
        return $this->fetchModels(function($model) use ($preload) {
            return $this->postLoad($model, $preload);
        });
    }
    
    /**
     * Fetches models from the database and processes them using a callback.
     *
     * @param callable $processModel A callback to process each model.
     * @return \Traversable          The processed models as a traversable object.
     */    
    private function fetchModels(callable $processModel): \Traversable
    {
        $factory = new Factory($this->schema::model());
    
        foreach ($this->fetchData() as $row) {
            $data = [];
    
            foreach ($row as $columnName => $columnValue) {
                if (array_key_exists($columnName, $this->schema::relations())) {
                    $columnValue = json_decode($columnValue, true);
                }
                $data[$columnName] = $columnValue;
            }
    
            $model = $factory->create($data);
            $processedModel = $processModel($model);
            yield $processedModel->id => $processedModel;
        }
    }    

    /**
     * Fetches raw data from the database based on query parameters.
     *
     * @return \Traversable An iterator of associative arrays representing each
     *                      row.
     */    
    private function fetchData(): \Traversable
    {
        $queryparams = $this->queryparams ?? new QueryParams();
        $query = (new QueryBuilder($this->schema, $queryparams))->build();
        $stmt = $this->db->prepare($query->toString());
        $stmt->execute($query->getParams());
    
        while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
            yield $row;
        }
    }
}
