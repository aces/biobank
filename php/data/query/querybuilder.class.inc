<?php declare(strict_types=1);  

namespace LORIS\biobank\data\query;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\DAO;

// TODO: I feel like there's redundancy between this and query params.
// There has to be a way to merge or make use of Query Params. Maybe a trait?
//
final class QueryBuilder
{
    protected Schema $schema;
    protected array $selects = [];
    protected array $conditions = [];
    protected ?string $groupBy = null;
    protected ?int $limit = null;
    protected ?int $offset = null;
    protected array $params = [];

    public function __construct(Schema $schema)
    {
        $this->schema = $schema;
    }

    public function reset(): self
    {
        $this->selects = [];
        $this->conditions = [];
        $this->groupBy = null;
        $this->limit = null;
        $this->offset = null;
        $this->params = [];
        return $this;
    }

    public function select(string ...$fields): self
    {
        $this->selects = $fields;
        return $this;
    }

    public function where(Condition ...$conditions): self
    {
        foreach ($conditions as $condition) {
            $this->conditions[] = $condition;
        }

        return $this;
    }

    public function groupBy(string $groupBy): self
    {
        $this->groupBy = $groupBy;
        return $this;
    }

    public function limit(?int $limit): self
    {
        $this->limit = $limit;
        return $this;
    }

    public function offset(?int $offset): self
    {
        $this->offset = $offset;
        return $this;
    }

    public function build(): Query
    {
        $query = $this->buildWith();
        $query .= $this->buildCommonTable();
        $query .= !is_null($this->limit) ? " LIMIT " . $this->limit : '';
        $query .= !is_null($this->offset) ? " OFFSET " . $this->offset : '';

        //$this->logQuery($query);

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
        foreach ($this->schema->relations as $relation) {
            if (isset($commonTables[$relation->schema->name])) {
                error_log('RECURSIVE COMMON TABLES');
                break;
            }

            $queryBuilder = new QueryBuilder($relation->schema);

            // TODO: I really am not super happy with this....
            foreach($this->conditions as $condition) {
                if ($condition->relation->name == $relation->schema->name) {
                    $queryBuilder->where(new Condition(
                        field: $condition->field,
                        value: $condition->value,
                    ));
                }
            }

            // Recursively build nested common tablesVx
            $nestedCommonTables = $queryBuilder->buildCommonTables($commonTables);
            $commonTables = array_merge($commonTables, $nestedCommonTables);

            // Build the common table for the current relation
            $commonTable = $queryBuilder->buildCommonTable();
            $commonTables[$relation->schema->name] = "{$relation->schema->name} AS ($commonTable)";
        }

        return $commonTables;
    }

    protected function buildCommonTable(): string
    {
        $schema = $this->schema;
        return "
            SELECT
                {$schema->table->name}.{$schema->id->name},
                {$this->buildFields()},
                {$this->buildJson()}
            FROM
                {$schema->table->name}
                {$this->buildJoins()}
                {$this->buildWhere()}
            GROUP BY
                {$schema->table->name}.{$schema->id->name}
        ";
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
        $relations = $schema->relations;

        // If no selects are specified, select all columns and relations.
        $selects = empty($this->selects)
            ? array_merge(array_keys($columns), array_keys($relations))
            : $this->selects;

        $fields = [];

        foreach ($selects as $select) {
            if (array_key_exists($select, $columns)) {
                // Select column from columns
                $column = $columns[$select]->name;
                $fields[] = "'{$select}', {$schema->table->name}.{$column}";
            }
    
            if (array_key_exists($select, $relations)) {
                // Select from relations
                $relation = $relations[$select];
                $fields[] = $relation->many
                    ? "'{$select}', JSON_ARRAYAGG(DISTINCT {$relation->schema->name}.json)"
                    : "'{$select}', {$relation->schema->name}.json";
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

        foreach ($this->schema->relations as $field => $relation) {
            $join = 'LEFT'; // Default join type is LEFT JOIN

            // Determine if the join type should be INNER JOIN based on conditions
            // TODO: this is part of the 'relation' condition I REALLY don't
            // like.
            foreach ($this->conditions as $condition) {
                if ($condition->relation->name == $relation->schema->name) {
                    $join = 'INNER';
                    break; // No need to check further once we determine it's an INNER JOIN
                }
            }

            $joinClauses[] = " {$join} JOIN {$relation->schema->name} ON 
                {$relation->from->table->alias}.{$relation->from->key->name} =
            {$relation->schema->name}.{$relation->schema->id->name}
            ";
        }

        return implode(' ', $joinClauses);
    }


    protected function buildWhere(): string
    {
        if (empty($this->conditions)) {
            return '';
        }

        $whereClauses = [];

        foreach ($this->conditions as $index => $condition) {
            $field = $condition->field;
            $value = $condition->value;

            if ($value === null) {
                continue;
            }

            $comparisonOperator = $condition->operator->toSql();
            $logicalOperator = $condition->logicalOperator->value;
            $placeholder = ":{$field}{$index}";

            // XXX: $condition->relation feels liek a hack. The idea is to add
            // filters to relations
            if (!isset($condition->relation)) {
                $table = $this->schema->table->name;
                $column = $this->schema->columns[$field]->name;

                $clause = "$table.$column $comparisonOperator $placeholder";

                // Add the logical operator for all clauses except the first one
                if (!empty($whereClauses)) {
                    $clause = "{$logicalOperator} {$clause}";
                }

                $whereClauses[] = $clause;
            }
            $this->params[$placeholder] = $value;
        }


        // TODO: this extra check for empty where is because of the residual conditions that
        // shouldn't  reallly be there.
        $whereClauseStr = ' WHERE '.implode('', $whereClauses);

        return !empty($whereClauses) ? $whereClauseStr : '';
    }
}
