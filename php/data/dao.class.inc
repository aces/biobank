<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data;                                                      
                                                                                   
use LORIS\biobank\Model;                                                           
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\data\query\QueryBuilder;                                         
use LORIS\biobank\data\query\QueryParams;                                          
use LORIS\biobank\data\query\Condition;                                            
use LORIS\Data\ProvisionerInstance;                                                
                                                                                   
class DAO                                             
{                                                                                  
    use Transactions;                                                              
    protected array $withs = [];
                                                                                   
    public function __construct(                                                   
        protected \Database $db,                                                   
        protected Schema $schema,                                                  
        protected Factory $factory                                                 
    ) {}                                                                             
                                                                                   
    public function find(QueryParams $queryParams): \Traversable //Model[]                
    {                                                                              
        $query = (new QueryBuilder($this->schema, $queryParams))->build();
        return new LORIS\Data\ModelProvisioner(
            $query->toString(),
            $query->getParams(),
        );
    }

    public function findLabels(QueryParams $queryParams): array
    {
        if (count($queryParams->fields) !== 1) {
            throw new InvalidArgumentException(
                "Only one field should be specified for findLabels"
            );
        }

        $query = (new QueryBuilder($this->schema, $queryParams))->build();

        return $this->db->pselectCol($query->toString(), $query->getParams());
    }

    public function with(string $relation): self
    {
        // Build the related schema dynamically
        $with = $this->schema->relations($relation);
        $with->buildSchema($this->schema->name, $relation);

        // Add the relation to the list of eager loads
        $this->withs[$relation] = $with;

        return $this;
    }

    // public function findById(string $id): ?Model
    // {
    //     $query = $this->queryBuilder
    //         ->where(new Condition(field: 'id', value: $id))
    //         ->build();

    //     return $this->db->pselectRow($query->toString(), $query->getParams());
    // }

    public function findByLabel(string $label): ?Model
    {
        $queryParams = (new QueryParams())
            ->where(new Condition(field: $this->schema->label, value: $label));

        foreach ($this->find($queryParams) as $model) {
            return $model;
        }

        return null; // Return null if no result is found
    }

    // protected function getIdbyLabel(string $label): int
    // {
    //     $query = $this->queryBuilder 
    //          ->select('id')
    //          ->where(new Condition(field: 'label', value: $label))
    //          ->build();

    //     $this->db->pselectOne($query, $this->queryBuilder->getParams());
    // }
    //

                                                                                  
    protected function mapInstanceToRow(Model $entity): array
    {
        $data = [];
        $fieldMappings = $this->schema::getFields();

        foreach ($fieldMappings as $property => $details) {
            if (isset($details['insert']) && $details['insert']) {

                if (method_exists($entity, $method)) {
                    $data[$details['field']] = $entity->$property;
                }
            }
        }

        return $data;
    }
}
