<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

class TableKey
{
    public function __construct(
        public Table $table,
        public string $key
    ) {}
}
