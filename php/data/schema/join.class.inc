<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

class Join
{
    public function __construct(
        public TableKey $from,
        public TableKey $to,
        public ?JoinType $type = JoinType::LEFT,
    ) {}
}
