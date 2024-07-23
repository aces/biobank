<?php declare(strict_types=1);                                                     
                                                                                   
namespace LORIS\biobank\data;                                                      
                                                                                   
                                                                                   
use LORIS\biobank\Model;                                                           
use LORIS\biobank\Factory;                                                         
use LORIS\biobank\data\query\QueryBuilder;                                         
use LORIS\biobank\data\query\QueryParams;                                          
use LORIS\biobank\data\query\Condition;                                            
use LORIS\Data\ProvisionerInstance;                                                
                                                                                   
class DAO extends ProvisionerInstance                                              
{                                                                                  
    use Transactions;                                                              
                                                                                   
    protected QueryBuilder $queryBuilder;                                          
                                                                                   
    public function __construct(                                                   
        protected \Database $db,                                                   
        protected Schema $schema,                                                  
        protected Factory $factory                                                 
    )                                                                              
    {                                                                              
        $this->queryBuilder = new QueryBuilder($schema);                           
    }                                                                              
                                                                                   
    protected function getAllInstances() : \Traversable                            
    {                                                                              
        return $this->find(new QueryParams());                                     
    }                                                                              

    private function decodeNestedJson($data) {
        if (is_array($data)) {
            foreach ($data as $key => $value) {
                if (is_string($value)) {
                    // Try decoding the string value as JSON
                    do {
                        $decodedValue = json_decode($value, true);
                        if (json_last_error() === JSON_ERROR_NONE) {
                            $value = $decodedValue; // Successfully decoded, replace the value
                        }
                    } while (json_last_error() === JSON_ERROR_NONE && is_string($value));
    
                    // Set the final value
                    $data[$key] = $value;
                } elseif (is_array($value)) {
                    // Recursively decode nested arrays
                    $data[$key] = $this->decodeNestedJson($value);
                }
            }
        }
        return $data;
    }    
                                                                                   
    public function find(QueryParams $queryParams): array //Model[]                
    {                                                                              
        $this->queryBuilder->reset();                                              
                                                                                   
        // TODO: ideally this would be $query = $this->queryBuilder($queryParams)->build();
        error_log('FIELDS: '.print_r($queryParams->fields, true));
        $query = $this->queryBuilder                                               
            ->select(...$queryParams->fields)                                      
            ->where(...$queryParams->conditions)                                   
            ->limit($queryParams->limit)                                           
            ->offset($queryParams->offset)                                         
            ->build();                                                             
                                                                                   
        $rows = $this->db->pselectWithIndexKey($query->toString(), $query->getParams(), $this->schema->label);       

        // error_log(print_r($rows, true));
                                                                                   
        $models = [];                                                              
        foreach ($rows as $row) {                                                  
            if (isset($row['json'])) {                                             
                $data = json_decode($row['json'], true);                           
                if (json_last_error() === JSON_ERROR_NONE && is_array($data)) {
                    $data = $this->decodeNestedJson($data); // Decode nested JSON strings
                    // error_log('Data passed to create: ' . print_r($data, true)); // Log the data passed to create
                    $models[] = $this->factory->create($data);
                } else {
                    error_log("JSON decode error: " . json_last_error_msg());
                    // Log the specific portion of data causing issues
                    $snippet_length = 20000; // Increase the length for better context
                    error_log("Problematic data snippet: " . substr($row['json'], 0, $snippet_length));
                }
            } else {                                                               
                error_log("Data field is missing in the row: " . print_r($row, true));
            }                                                                      
        }                                                                          
                                                                                   
        return $models;                                                            
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
        return $this->find(
            new QueryParams(
                conditions: [
                    new Condition(field: $this->schema->label, value: $label)
                ]
            )
        )[0];
    }

    // protected function getIdbyLabel(string $label): int
    // {
    //     $query = $this->queryBuilder 
    //          ->select('id')
    //          ->where(new Condition(field: 'label', value: $label))
    //          ->build();

    //     $this->db->pselectOne($query, $this->queryBuilder->getParams());
    // }

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
