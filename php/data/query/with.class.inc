<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

class With
{
    public function __construct(
        public string $name,
        public array $conditions = [],
        public array $subIncludes = []
    ) {}
}
