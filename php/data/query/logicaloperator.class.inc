<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

enum LogicalOperator: string
{
    case AND = 'AND';
    case OR = 'OR';
}

