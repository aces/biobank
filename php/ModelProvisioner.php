<?php

namespace LORIS\Data\Provisioners;

use \LORIS\biobank\data\query\Query;
use \LORIS\biobank\data\query\With;
use \LORIS\biobank\data\query\QueryParams;
use \LORIS\biobank\data\query\QueryBuilder;
use \LORIS\biobank\data\Schema;
use \LORIS\Data\Factory;

final class ModelProvisioner extends \LORIS\Data\ProvisionerInstance
{
    private array $withs = [];
    private ?QueryParams $queryparams = null;

    /**
     * Constructor
     *
     * @param string $schema The schema that defines the returned objects
     */
    public function __construct(
        private Schema $schema
    ) {}

    public function get(?QueryParams $queryparams): self
    {
        $this->queryparams = $queryparams;
        return $this;
    }

    public function with(With ...$relations): self
    {
        // Append new relations to the existing $withs array
        $this->withs = array_merge($this->withs, $relations);
        return $this;
    }

    /**
     * GetAllInstances executes the query with PDO Fetch function
     * and creates instances of the class with the fetched data.
     *
     * @return \Traversable
     */
    public function getAllInstances() : \Traversable
    {
        $queryparams = $this->queryparams ?? new QueryParams();
        $query = (new QueryBuilder($this->schema, $queryparams))->build();
        $DB   = (\NDB_Factory::singleton())->database();
        $stmt = $DB->prepare($query->toString());
        $stmt->execute($query->getParams());
        $factory = new Factory($this->schema->model);

        // Use a generator to yield each row as it's fetched and processed
        while ($row = $stmt->fetch(\PDO::FETCH_ASSOC)) {
            $data = [];
            error_log('ORINGAL ARRAY:'.print_r($row, true));
            // Process each column in the row
            foreach ($row as $columnName => $columnValue) {
                if (array_key_exists($columnName, $this->schema->relations)) {
                    $columnValue = json_decode($columnValue, true);
                }
                $data[$columnName] = $columnValue;
            }
            error_log('DECODED ARRAY:'.print_r($data, true));
            
            // Yield the created model instance
            yield $factory->create($data);
        }
    }
}
