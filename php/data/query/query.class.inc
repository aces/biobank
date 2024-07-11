<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

class Query
{
    public function __construct(
        private string $string,
        private array $params
    ) {}


    public function toString(): string
    {
        return $this->string;
    }

    public function getParams(): array
    {
        return $this->params;
    }
}
