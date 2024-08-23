<?php declare(strict_types=1);  

namespace LORIS\biobank\data\query;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schema\JoinType;

final class QueryBuilder
{
    protected array $params = [];

    public function __construct(
        protected Schema $schema,
        protected QueryParams $queryParams,
    ) {
        $this->validateQueryParams();
    }

    // TODO: NOT WORKING FOR SOME REASON, MAKE SURE THIS GETS SENT TO THE FRONT
    // END — ALSO, CONSIDER A BETTER LOCATION FOR THIS VALIDATION, SUCH AS IN
    // THE QUERYPARAMS THEMSELVES, ALTHOUGH I'M NOT CONVINCED.
    private function validateQueryParams(): void
    {
        foreach ($this->queryParams->conditions as $condition) {
            if (!isset($this->schema->columns[$condition->field])) {
                throw new \InvalidArgumentException("Field '{$condition->field}' does not exist in the schema.");
            }
        }

        foreach ($this->queryParams->includes as $include) {
            if (!isset($this->schema->relations[$include->name])) {
                throw new \InvalidArgumentException("Relation '{$include->name}' does not exist in the schema.");
            }
        }
    }

    public function build(): Query
    {
        $query = $this->buildCommonTable() .
           $this->buildLimitOffset();

        $this->logQuery($query);

        return new Query(string: $query, params: $this->params);
    }

    private function logQuery(string $query): void
    {
        $chunkSize = 1000; // Set chunk size based on your needs
        $queryLength = strlen($query);

        for ($i = 0; $i < $queryLength; $i += $chunkSize) {
            error_log(substr($query, $i, $chunkSize));
        }
    }

    private function buildWith()
    {
        $commonTables = $this->buildCommonTables();
        if (empty($commonTables)) {
            return '';
        }

        $ctes = implode(', ', $commonTables);

        return "WITH $ctes ";
    }

    private function buildCommonTables(array $commonTables = []): array
    {
        foreach ($this->queryParams->includes as $include) {

            $relation = $this->schema->relations[$include->name];

            // Build the schema for the current relation
            $relation->buildSchema($this->schema->name, $include->name);

            // Prepare QueryParams for the current relation, handling conditions and sub-includes
            $queryParams = new QueryParams(
                conditions: $include->conditions,
                includes: $include->subIncludes
            );

            // Create a new QueryBuilder instance for the relation's schem
            $queryBuilder = new QueryBuilder($relation->schema, $queryParams);

            // Recursively build common tables using the new QueryBuilder
            $nestedCommonTables = $queryBuilder->buildCommonTables($commonTables);
            $commonTables = array_merge($commonTables, $nestedCommonTables);

            // Add the common table for the current relation
            $commonTable = $queryBuilder->buildCommonTable();
            $commonTables[$relation->schema->name] = "{$relation->schema->name} AS ($commonTable)";

            // Merge parameters from the nested query builder back into this one
            $this->params = array_merge($this->params, $queryBuilder->params);
        }

        return $commonTables;
    }

    protected function buildCommonTable(): string
    {
        $schema = $this->schema;
    
        // Handle the select clause
        $selectClause = $this->queryParams->countOnly
            ? "COUNT(*) AS row_count"
            : $this->buildSelect();
    
        // Build the query
        return "
            SELECT
                $selectClause
            FROM
                {$schema->table->name}
                {$this->buildJoins()}
                {$this->buildWhere()}
            " . $this->buildGroupBy();
    }
    
    private function buildSelect(): string
    {
        $schema = $this->schema;

        return $this->buildFields();
    
        // if (empty($this->queryParams->fields)) {
        //     return "{$schema->table->name}.{$schema->id->name}," .
        //            $this->buildFields();
        // }
    
        // return implode(", ", array_map(
        //     fn($field) => "{$schema->table->name}.{$schema->columns[$field]->name} AS {$field}",
        //     $this->queryParams->fields
        // ));
    }
    
    private function buildGroupBy(): string
    {
        return $this->queryParams->countOnly ?
            "" :
            "GROUP BY {$this->schema->table->name}.{$this->schema->id}";
    }    


    public function buildFields(): string                                       
    {                                                                           
        $schema = $this->schema;                                                
        $columns = $schema->columns;                                            
        $fields = [];                                                           
                                                                                
        foreach ($columns as $field => $column) {                               
            $fields[] = "{$schema->table->name}.{$schema->id} AS id";     
            $fields[] = "{$schema->table->name}.{$column->name} AS $field";     
        }                                                                       
                                                                                
        foreach ($schema->relations as $name => $relation) {                    
                                                                                
                                                                                
            $relation->buildSchema($this->schema->name, $name);        
                                                                                
            // If the relation is "one", include all columns from the related schema
            foreach ($relation->schema->columns as $field => $column) {         
                $jsonFields[] = "'id', {$relation->schema->name}.{$relation->schema->id}";
                $jsonFields[] = "'$field', {$relation->schema->name}.{$column->name}";
            }                                                                   
                                                                                
            $fields[] = $relation->many                                   
                ? "JSON_ARRAYAGG(JSON_OBJECT(" . implode(", ", $jsonFields) . ")) as {$name}"
                : "JSON_OBJECT(" . implode(", ", $jsonFields) . ") as {$name}";
        }                                                                       
                                                                                
        return implode(", ", $fields);                                          
    }              


    protected function buildJoins(): string
    {
        $schema = $this->schema;
        $joinClauses = [];

        foreach ($schema->joins as $join) {
            $joinClauses[] = " {$join->type->value} JOIN
                {$join->to->table->name} {$join->to->table->alias} ON
                {$join->from->table->name}.{$join->from->key} =
                {$join->to->table->alias}.{$join->to->key}";
        }

        foreach ($schema->relations as $name => $relation) {
            $joinType = JoinType::LEFT; // Default join type is LEFT JOIN

            $joinClauses[] = " {$joinType->value}
                JOIN {$relation->schema->table->name} as {$relation->schema->name}
                ON {$relation->from->table->alias}.{$relation->from->key} =
            {$relation->schema->name}.{$relation->schema->id}
            ";
        }

        return implode(' ', $joinClauses);
    }

    protected function buildWhere(): string
    {
        $whereClauses = [];
    
        // Function to process a condition and return the SQL clause
        $processCondition = function($condition, $index, $schema) use (&$whereClauses) {
            $field = $condition->field;
            $value = $condition->value;
    
            if ($value === null) {
                return;
            }
    
            $column = $schema->columns[$field]->name;
            $alias = $schema->name;
            $comparisonOperator = $condition->operator->toSql();
            $placeholder = ":{$field}{$index}";
    
            $clause = "$alias.$column $comparisonOperator $placeholder";
    
            if (!empty($whereClauses)) {
                $logicalOperator = $condition->logicalOperator->value;
                $clause = "{$logicalOperator} {$clause}";
            }
    
            $whereClauses[] = $clause;
            $this->params[$placeholder] = $value;
        };
    
        // Process primary conditions
        foreach ($this->queryParams->conditions as $index => $condition) {
            $tableAlias = $this->schema->table->name;
            $processCondition($condition, $index, $this->schema);
        }
    
        // Process includes conditions with relations
        foreach ($this->queryParams->includes as $includeIndex => $include) {
            $schema = $this->schema->relations[$include->name]->schema;
    
            // Iterate over each condition within the includes array
            foreach ($include->conditions as $conditionIndex => $condition) {
                $processCondition($condition, "include{$includeIndex}_{$conditionIndex}", $schema);
            }
        }
    
        // Combine all where clauses
        $whereClauseStr = ' WHERE ' . implode(' ', $whereClauses);
        return !empty($whereClauses) ? $whereClauseStr : '';
    }

    private function buildLimitOffset(): string
    {
        if (is_null($this->queryParams->limit)) {
            return '';
        }

        $limitOffset = " LIMIT " . $this->queryParams->limit;
        // $limitOffset = " LIMIT 1";

        if (!is_null($this->queryParams->offset)) {
            $limitOffset .= " OFFSET " . $this->queryParams->offset;
        }

        return $limitOffset;
    }

}
