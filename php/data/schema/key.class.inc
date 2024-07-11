<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

class Key extends Column
{
    public function __construct(
        string $name,
    ) {
        parent::__construct($name, ColumnType::INT);
    }
}
