<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

use LORIS\biobank\data\Schema;

class Relation
{
    public function __construct(
        public TableKey $from,
        public Schema $to,
        public ?string $alias = null,
        public ?bool $many = false
    ) {}
}

