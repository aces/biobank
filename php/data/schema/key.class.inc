<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

class Key extends Column
{
    public function __construct(
        string $name,
        Table $table,
    ) {
        parent::__construct($name, $table);
    }
}
