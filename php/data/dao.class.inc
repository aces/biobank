<?php declare(strict_types=1);  

namespace LORIS\biobank\data;

use LORIS\Data\ProvisionerInstance;
use LORIS\biobank\Model;
use LORIS\biobank\data\query\QueryBuilder;
use LORIS\biobank\data\query\QueryParams;

class DAO extends ProvisionerInstance
{
    use Transactions; 

    protected QueryBuilder $queryBuilder;

    public function __construct(
        protected \Database $db, 
        protected Schema $schema
    ) 
    {
        $this->queryBuilder = new QueryBuilder($schema);
    }

    protected function getAllInstances() : \Traversable
    {
        $query = $this->queryBuilder->build();
        return $this->db->pselect($query->toString(), $query->getParams());
    }

    public function find(QueryParams $queryParams)
    {
        $query = $this->queryBuilder
            ->select(...$queryParams->fields)
            ->where(...$queryParams->conditions)
            ->limit($queryParams->limit)
            ->offset($queryParams->offset)
            ->build();

        return $this->db->pselect($query->toString(), $query->getParams());
    }

    public function findById(string $id): ?Model
    {
        $query = $this->queryBuilder
            ->where(new Condition(field: 'id', value: $id))
            ->build();

        return $this->db->pselectRow($query->toString(), $query->getParams());
    }

    public function findByLabel(string $label): ?Model
    {
        $query = $this->queryBuilder
            ->where(new Condition(field: 'label', value: $label))
            ->build();

        return $this->db->pselectRow($query->toString(), $query->getParams());
    }

    protected function getIdbyLabel(string $label): int
    {
        $query = $this->queryBuilder 
             ->select('id')
             ->where(new Condition(field: 'label', value: $label))
             ->build();

        $this->db->pselectOne($query, $this->queryBuilder->getParams());
    }

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
