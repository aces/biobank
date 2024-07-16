<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

class QueryParams {
    public function __construct(
        public ?array $fields = [],
        public ?array $conditions = [], //Condition[]
        public ?array $excludeConditions = [],
        public ?int $limit = null,
        public ?int $offset = null,
        public ?array $orderBy = null,
    ) {}
}
