<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

class QueryParams {
    public function __construct(
        public ?array $fields = [],
        public ?array $conditions = [], //Condition[]
        public ?array $excludeConditions = [],
        public ?array $includes = [],
        public ?int $limit = null,
        public ?int $offset = null,
        public ?array $orderBy = null,
        public bool $countOnly = false,
    ) {}

    public function select(string ...$fields): self
    {
        $this->fields = $fields;
        return $this;
    }

    public function where(Condition ...$conditions): self
    {
        foreach ($conditions as $condition) {
            $this->conditions[] = $condition;
        }
        return $this;
    }

    public function include(With ...$includes): self
    {
        $this->includes = array_merge($this->includes, $includes);
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

    public function countOnly(bool $countOnly): self
    {
        $this->countOnly = $countOnly;
        return $this;
    }

    public function hasAnyParamsSet(): bool
    {
        return $this->countOnly ||
               !empty($this->conditions) ||
               !empty($this->fields) ||
               !empty($this->includes) ||  // If you consider includes as a meaningful param
               isset($this->limit) ||
               isset($this->offset);
    }
}
