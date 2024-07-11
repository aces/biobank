<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

class Condition
{
    public function __construct(
        public string $field,
        public $value,
        public ?Operator $operator = Operator::EQUALS
        public ?LogicalOperator $logicalOperator = LogicalOperator::AND
    )
    {}
}
