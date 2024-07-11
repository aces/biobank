<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

enum Operator: string
{
    case EQUALS = 'eq';                    // Equals
    case NOT_EQUALS = 'neq';               // Not equals
    case GREATER_THAN = 'gt';              // Greater than
    case LESS_THAN = 'lt';                 // Less than
    case GREATER_THAN_OR_EQUALS = 'gte';   // Greater than or equals
    case LESS_THAN_OR_EQUALS = 'lte';      // Less than or equals
    case LIKE = 'like';                    // Like (pattern match)
    case IN = 'in';                        // In (value in set)
}
