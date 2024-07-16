<?php declare(strict_types=1);  

namespace LORIS\biobank\data\query;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schema\LoadingStrategy;

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
        $this->selectFields = [];
        $this->conditions = [];
        $this->limit = null;
        $this->offset = null;
        $this->params = [];
        $this->queryString = '';
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
        $query .= "SELECT {$this->buildFields()} FROM {$this->schema::getTable()->name}";
        $query .= $this->buildJoins();
        $query .= $this->buildWhere();
        //$query .= !is_null($this->groupBy) ? " GROUP BY " . $this->groupBy : '';
        $query .= " GROUP BY {$this->schema::getTable()->name}.{$this->schema::getId()->name}";
        $query .= !is_null($this->limit) ? " LIMIT " . $this->limit : '';
        $query .= !is_null($this->offset) ? " OFFSET " . $this->offset : '';

        return new Query(string: $query, params: $this->params);
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
        if ($this->schema->isLazy()) {
            return [];
        }

        foreach ($this->schema::getRelations() as $name => $relation) {
            $schema = $relation->to;

            // Recursively build relations for nested DAOs
            $queryBuilder = new QueryBuilder($schema);
            // foreach($this->where)

            $nestedCommonTables = $queryBuilder->buildCommonTables();
            foreach ($nestedCommonTables as $nestedCommonTable) {
                if (!in_array($nestedCommonTable, $commonTables)) {
                    $commonTables[] = $nestedCommonTable;
                }
            }

            $commonTable = $queryBuilder->buildCommonTable();
            // $queryBuilder->where
            // $queryBuilder->buildWhere()
            $commonTables[] = "$name AS ($commonTable)";
        }

        return $commonTables; // Ensure unique CTEs
    }

    protected function buildCommonTable(): string
    {
        $schema = $this->schema;
        return "
            SELECT
                {$schema::getTable()->name}.{$schema::getId()->name},
                {$this->buildFilterable()},
                {$this->buildFields()}
            FROM
                {$schema::getTable()->name}
                {$this->buildJoins()}
        ";
    }

    public function buildFilterable(): string
    {
        $schema = $this->schema;
        $columns = $schema::getColumns();
        $fields = [];
    
        foreach ($columns as $alias => $column) {
            $fields[] = "{$schema::getTable()->name}.{$column->name} AS $alias";
        }
    
        return implode(", ", $fields);
    }

    public function buildFields(): string
    {
        $schema = $this->schema;

        $columns = $schema::getColumns();
        $relations = $schema->isEager() ? $schema::getRelations() : [];

        // If no selects are specified, select all columns and relations.
        $selects = empty($this->selects)
            ? array_merge(array_keys($columns), array_keys($relations))
            : $this->selects;

        $fields = [];

        foreach ($selects as $select) {
            if (array_key_exists($select, $columns)) {
                // Select column from columns
                $column = $columns[$select]->name;
                $fields[] = "'{$select}', {$schema::getTable()->name}.{$column}";
            }
    
            if ($schema->isLazy()) {
                continue;
            }

            if (array_key_exists($select, $relations)) {
                // Select from relations
                $fields[] = $relations[$select]->many
                    ? "'{$select}', JSON_ARRAYAGG($select.data)"
                    : "'{$select}', $select.data";
            }
        }
    
        return "JSON_OBJECT(" . implode(", ", $fields) . ") AS data";
    }

    protected function buildJoins(): string
    {
        // if (empty($this->schema::getJoins() && !$this->schema->isEager())) {
        //     return '';
        // }

        $joinClauses = [];

        foreach ($this->schema::getJoins() as $name => $join) {
            $joinClauses[] = " {$join->type->value} JOIN
                {$join->to->table->name} {$join->to->table->alias} ON
                {$join->from->table->name}.{$join->from->key->name} =
                {$join->to->table->alias}.{$join->to->key->name}";
        }

        if ($this->schema->isEager()) {
            foreach ($this->schema::getRelations() as $name => $relation) {
                $joinClauses[] = " LEFT JOIN {$name} ON 
                    {$relation->from->table->alias}.{$relation->from->key->name} =
                    $name.{$relation->to::getId()->name}
                ";
            }
        }

        return implode(' ', $joinClauses);
    }


    protected function buildWhere(): string
    {
        if (empty($this->conditions)) {
            return '';
        }

        $whereClause = [];
        $queryParams = [];

        foreach ($this->conditions as $index => $condition) {
            $field = $condition->field;
            $value = $condition->value;
            $relation = $condition->relation;

            if ($value === null) {
                continue;
            }

            $comparisonOperator = $condition->operator->toSql();
            $logicalOperator = $condition->logicalOperator->value;
            $placeholder = ":param{$index}";

            // Prefix the field with the relation alias if it exists
            if ($relation) {
                $field = "{$relation->alias}.{$field}";
            }

            $clause = "$field $comparisonOperator $placeholder";

            // Add the logical operator for all clauses except the first one
            if (!empty($whereClauses)) {
                $clause = "{$logicalOperator} {$clause}";
            }
            $whereClause[] = $clause;
            $queryParams[$placeholder] = $value;
        }

        $whereClauseStr = ' WHERE '.implode('', $whereClause);

        $this->params = $queryParams;
        return $whereClauseStr;
    }
}
