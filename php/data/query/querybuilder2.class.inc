<?php declare(strict_types=1);  

namespace LORIS\biobank\data\query;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schema\JoinType;

final class QueryBuilder2
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
        $query = $this->buildWith() .
           $this->buildCommonTable() .
           $this->buildLimitOffset();

        //$this->logQuery($query);
        // error_log(print_r($this->params, true));

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
    
        if (empty($this->queryParams->fields)) {
            return "{$schema->table->name}.{$schema->id->name}, " .
                   $this->buildFields() . ", " .
                   $this->buildJson();
        }
    
        return implode(", ", array_map(
            fn($field) => "{$schema->table->name}.{$schema->columns[$field]->name} AS {$field}",
            $this->queryParams->fields
        ));
    }
    
    private function buildGroupBy(): string
    {
        return $this->queryParams->countOnly ?
            "" :
            "GROUP BY {$this->schema->table->name}.{$this->schema->id->name}";
    }    

    public function buildFields(): string
    {
        $schema = $this->schema;
        $columns = $schema->columns;
        $fields = [];
    
        foreach ($columns as $field => $column) {
            $fields[] = "{$schema->table->name}.{$column->name} AS $field";
        }
    
        return implode(", ", $fields);
    }

    public function buildJson(): string
    {
        $schema = $this->schema;

        $columns = $schema->columns;

        // If no selects are specified, select all columns.
        $selects = empty($this->queryParams->fields)
            ? array_keys($columns)
            : $this->queryParams->fields;

        $fields = [];

        foreach ($selects as $select) {
            if (array_key_exists($select, $columns)) {
                // Select column from columns
                $column = $columns[$select]->name;
                $fields[] = "'{$select}', {$schema->table->name}.{$column}";
            }
        }

        foreach ($this->queryParams->includes as $include) {
            if (array_key_exists($include->name, $schema->relations)) {
                // Select from relations
                $relation = $schema->relations[$include->name];
                $fields[] = $relation->many
                    ? "'{$include->name}', JSON_ARRAYAGG(DISTINCT {$relation->schema->name}.json)"
                    : "'{$include->name}', {$relation->schema->name}.json";
            }
        }
    
        return "JSON_OBJECT(" . implode(", ", $fields) . ") AS json";
    }

    protected function buildJoins(): string
    {
        $joinClauses = [];

        foreach ($this->schema->joins as $join) {
            $joinClauses[] = " {$join->type->value} JOIN
                {$join->to->table->name} {$join->to->table->alias} ON
                {$join->from->table->name}.{$join->from->key->name} =
                {$join->to->table->alias}.{$join->to->key->name}";
        }

        foreach ($this->queryParams->includes as $include) {
            $relation = $this->schema->relations[$include->name];
            $join = $include->conditions ? JoinType::INNER : JoinType::LEFT; // Default join type is LEFT JOIN

            $joinClauses[] = " {$join->value} JOIN {$relation->schema->name} ON 
                {$relation->from->table->alias}.{$relation->from->key->name} =
            {$relation->schema->name}.{$relation->schema->id->name}
            ";
        }

        return implode(' ', $joinClauses);
    }


    protected function buildWhere(): string
    {
        if (empty($this->queryParams->conditions)) {
            return '';
        }

        $whereClauses = [];

        foreach ($this->queryParams->conditions as $index => $condition) {
            $field = $condition->field;
            $value = $condition->value;

            // TODO: this should maybe be taken out
            if ($value === null) {
                continue;
            }

            $table = $this->schema->table->name;
            $column = $this->schema->columns[$field]->name;
            $comparisonOperator = $condition->operator->toSql();
            $placeholder = ":{$field}{$index}";

            $clause = "$table.$column $comparisonOperator $placeholder";

            // Add the logical operator for all clauses except the first one
            if (!empty($whereClauses)) {
                $logicalOperator = $condition->logicalOperator->value;
                $clause = "{$logicalOperator} {$clause}";
            }

            $whereClauses[] = $clause;

            $this->params[$placeholder] = $value;
        }


        // TODO: this extra check for empty where is because of the residual conditions that
        // shouldn't  reallly be there.
        $whereClauseStr = ' WHERE '.implode('', $whereClauses);

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
