<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

use LORIS\biobank\data\Schema;

class Relation
{
    public readonly Schema $schema;

    public function __construct(
        public TableKey $from,
        private string $to, // schema fully qualified class name
        private ?LoadingStrategy $strategy = LoadingStrategy::LAZY,
        public ?bool $many = false,
    ) {
        if (!is_subclass_of($to, Schema::class)) {
            throw new \InvalidArgumentException(
                'The "to" parameter must be a subclass of Schema.'
            );
        }
    }

    public function buildSchema(string $parentName, string $fieldName): void
    {
        $this->schema = new $this->to(
            name: $parentName . '_' . $fieldName,
            strategy: $this->strategy
        );
    }
}

