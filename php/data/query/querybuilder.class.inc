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
        $query .= !is_null($this->groupBy) ? " GROUP BY " . $this->groupBy : '';
        $query .= !is_null($this->limit) ? " LIMIT " . $this->limit : '';
        $query .= !is_null($this->offset) ? " OFFSET " . $this->offset : '';
        error_log($query);

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

            $nestedCommonTables = $queryBuilder->buildCommonTables();
            foreach ($nestedCommonTables as $nestedCommonTable) {
                if (!in_array($nestedCommonTable, $commonTables)) {
                    $commonTables[] = $nestedCommonTable;
                }
            }

            $commonTable = $queryBuilder->buildCommonTable();
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
                {$this->buildFields()}
            FROM
                {$schema::getTable()->name}
                {$this->buildJoins()}
        ";
    }

    public function buildFields(): string
    {
        $schema = $this->schema;
        $columns = $schema->getFields();
        if (empty($this->selects)) {
            $this->selects = $columns;
        }
    
        $fields = [];
    
        foreach ($this->selects as $select) {
            if (array_key_exists($select, $columns)) {
                $column = $columns[$select]->name;
                $fields[] = "'{$select}', {$schema::getTable()->name}.{$column}";
            }
    
            if ($schema->isLazy()) {
                continue;
            }

            $relations = $schema::getRelations();
            if (array_key_exists($select, $relations)) {
                $fields[] = $relations[$select]->many
                    ? "'{$select}', JSON_ARRAYAGG($select.data)"
                    : "'{$select}', $select.data";
            }
        }
    
        return "JSON_OBJECT(" . implode(",\n", $fields) . ") AS data";
    }

    protected function buildJoins(): string
    {
        if (empty($this->schema::getJoins())) {
            return '';
        }

        $joinClauses = [];

        foreach ($this->schema::getJoins() as $name => $join) {
            $joinClauses[] = " {$join->type->value} JOIN {$join->to->table->name} $name
                ON {$join->from->table->name}.{$join->from->key->name} = $name.{$join->to->key->name}";
        }

        if ($this->schema->isEager()) {
            foreach ($this->schema::getRelations() as $name => $relation) {
                $joinClauses[] = " LEFT JOIN {$name} ON 
                    {$relation->from->table->name}.
                    {$relation->from->key->name} =
                    $name.{$relation->to::getTable()->name}
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

            if ($value === null) {
                continue;
            }

            $comparisonOperator = $condition->operator->value;
            $logicalOperator = $condition->logicalOperator->value;
            $placeholder = ":param{$index}";

            $clause .= "$field $comparisonOperator $placeholder";
            $whereClause[] = $clause;
            $queryParams[$placeholder] = $value;
        }

        $whereClauseStr = ' WHERE '.implode('', $whereClause);

        $this->params = $queryParams;
        return $whereClnuseStr;
    }
}
