<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

/**
 * Class Join
 *
 * Represents a SQL join between two tables, defined by their keys.
 * By default, the join type is set to `LEFT`, but it can be modified
 * using the optional `$type` parameter.
 *
 * @package    LORIS
 */
class Join
{
    /**
     * Constructs a Join instance.
     *
     * @param TableKey  $from The key from the source table.
     * @param TableKey  $to   The key from the target table.
     * @param ?JoinType $type The type of join, defaulting to a LEFT join.
     */    
    public function __construct(
        public TableKey $from,
        public TableKey $to,
        public ?JoinType $type = JoinType::LEFT,
    ) {}
}
