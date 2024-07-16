<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

final class QueryParamsHandler
{
    protected $queryParams;

    public function __construct(array $queryParams)
    {
        $this->queryParams = $queryParams;
    }

    public function parse(): QueryParams
    {
        return new QueryParams(
            fields: $this->parseFields(),
            conditions: $this->parseConditions(),
            excludeConditions: $this->parseExcludeConditions(),
            limit: $this->parseLimit(),
            offset: $this->parseOffset(),
            orderBy: $this->parseOrderBy(),
        );
    }

    protected function parseFields(): array
    {
        return !empty($this->queryParams['fields']) ? 
            explode(',', $this->queryParams['fields']) :
            [];
    }

    protected function parseConditions(): array
    {
        return $this->parseComplexConditions('conditions');
    }

    protected function parseExcludeConditions(): array
    {
        return $this->parseComplexConditions('exclude_conditions');
    }

    protected function parseComplexConditions(string $key): array
    {
        $conditions = [];

        foreach ($this->queryParams[$key] ?? [] as $field => $value) {
            [$operatorStr, $logicalOperatorStr, $field] = $this->parseFieldWithOperators($field);

            $operator = Operator::from($operatorStr);
            $logicalOperator = LogicalOperator::from($logicalOperatorStr);

            $conditions[] = new Condition(
                $field,
                $value,
                $operator,
                $logicalOperator
            );
        }

        return $conditions;
    }

    protected function parseFieldWithOperators(string $field): array
    {
        $parts = explode('_', $field, 3);
        $operatorStr = $parts[0];
        $logicalOperatorStr = $parts[1];
        $field = $parts[2] ?? '';

        return [$operatorStr, $logicalOperatorStr, $field];
    }


    //TODO: I feel like there's a better way to make these functions.
    protected function parseLimit(): ?int
    {
        return isset($this->queryParams['limit']) && is_numeric($this->queryParams['limit'])
            ? (int) $this->queryParams['limit']
            : null;
    }

    protected function parseOffset(): ?int
    {
        return isset($this->queryParams['offset']) && is_numeric($this->queryParams['offset'])
            ? (int) $this->queryParams['offset']
            : null;
    }

    protected function parseOrderBy(): array
    {
        $orderBy = [];

        foreach (explode(',', $this->queryParams['order_by'] ?? '') as $field) {
            if (isset($field[0]) && $field[0] === '-') {
                $orderBy[substr($field, 1)] = 'DESC';
            } else {
                $orderBy[$field] = 'ASC';
            }
        }

        return $orderBy;
    }
}
