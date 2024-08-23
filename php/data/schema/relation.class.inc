<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

use LORIS\biobank\data\Schema;

enum RelationType: string
{
    case OnetoOne = 'OnetoOne';
    case OnetoMany = 'OnetoMany';
    case ManytoOne = 'ManytoOne';
    case ManytoMany = 'ManytoMany';
}

class Relation
{
    public readonly Schema $schema;

    public function __construct(
        public TableKey $from,
        private string $to, // schema fully qualified class name
        public bool $many = false,
        public ?RelationType $type = null,
    ) {
        if (!is_subclass_of($to, Schema::class)) {
            throw new \InvalidArgumentException(
                'The "to" parameter must be a subclass of Schema.'
            );
        }
    }

    public function buildSchema(string $parentName, string $fieldName): self
    {
        $this->schema = new $this->to(
            name: $parentName . '_' . $fieldName,
        );
        return $this;
    }
}

