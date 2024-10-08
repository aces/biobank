<?php declare(strict_types=1);  

namespace LORIS\biobank\data\query;

use LORIS\biobank\data\Schema;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schema\JoinType;

/**
 * Class QueryBuilder
 *
 * Builds SQL queries based on schema and query parameters, allowing for
 * dynamic joins, where clauses, and field selection. It constructs the
 * necessary SQL commands based on conditions and relations specified in
 * `QueryParams`.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @final
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public License
 */
final class QueryBuilder
{
    /**
     * Stores query parameters to be used in the built SQL query.
     *
     * @var array
     */    
    protected array $params = [];

    /**
     * Constructs the QueryBuilder.
     *
     * @param Schema      $schema      The schema defining the table and fields.
     * @param QueryParams $queryParams The query parameters containing conditions
     *                                 and relations.
     */    
    public function __construct(
        protected Schema $schema,
        protected QueryParams $queryParams,
    ) {
        $this->validateQueryParams();
    }

    // TODO: NOT WORKING FOR SOME REASON, MAKE SURE THIS GETS SENT TO THE FRONT
    // END — ALSO, CONSIDER A BETTER LOCATION FOR THIS VALIDATION, SUCH AS IN
    // THE QUERYPARAMS THEMSELVES, ALTHOUGH I'M NOT CONVINCED.
    /**
     * Validates that the query parameters reference valid schema columns and
     * relations. Throws exceptions for invalid fields or relations.
     *
     * @throws \InvalidArgumentException If a field or relation is invalid.
     * @return void
     */    // 
    private function validateQueryParams(): void
    {
        foreach ($this->queryParams->conditions as $condition) {
            if (!isset($this->schema::columns()[$condition->field])) {
                throw new \InvalidArgumentException("Field '{$condition->field}' does not exist in the schema.");
            }
        }

        foreach ($this->queryParams->includes as $include) {
            if (!isset($this->schema::relations()[$include->name])) {
                throw new \InvalidArgumentException("Relation '{$include->name}' does not exist in the schema.");
            }
        }
    }

    /**
     * Builds the SQL query based on the schema and query parameters.
     *
     * @return Query The constructed Query object containing the SQL string and
     *               parameters.
     */    
    public function build(): Query
    {
        $query = "
            SELECT
                {$this->buildFields()}
            FROM
                {$this->schema::table()->name} {$this->schema::table()->alias}
                {$this->buildJoins()}
                {$this->buildWhere()}
            " . $this->buildGroupBy() . $this->buildLimitOffset();

        // $this->logQuery($query);

        return new Query(string: $query, params: $this->params);
    }

    private function logQuery(string $query): void
    {
        $chunkSize = 1000; // Set chunk size based on your needs
        $queryLength = strlen($query);

        for ($i = 0; $i < $queryLength; $i += $chunkSize) {
            error_log(substr($query, $i, $chunkSize));
        }
    }
    
    /**
     * Builds the GROUP BY clause for the query.
     *
     * @return string The GROUP BY clause.
     */    
    private function buildGroupBy(): string
    {
        return "GROUP BY {$this->schema::table()->alias}.{$this->schema::id()}";
    }    

    /**
     * Builds the SELECT fields for the query.
     *
     * @return string The SELECT fields clause.
     */
    public function buildFields(): string                                       
    {                                                                           
        $schema = $this->schema;                                                
        $columns = $schema::columns();                                            
        $fields = [];                                                           
                                                                                
        $fields[] = "{$this->schema::table()->alias}.{$this->schema::id()} AS id";
        foreach ($columns as $field => $column) {                               
            $fields[] = "{$column->table->alias}.{$column->name} AS $field";     
        }                                                                       
                                                                                
        foreach ($schema::relations() as $name => $relation) {                    
            $jsonFields = []; // Reset $jsonFields for each relation
                                                                                
            $jsonFields[] = "'id', $name.{$relation->to::id()}";
            foreach ($relation->to::columns() as $field => $column) {         
                if ($field == $relation->to::label()) {
                    $jsonFields[] = "'$field', $name.{$column->name}";
                }
            }                                                                   
                                                                                
            $fields[] = $relation->many                                   
                ? "JSON_ARRAYAGG(JSON_OBJECT(" . implode(", ", $jsonFields) . ")) as {$name}"
                : "JSON_OBJECT(" . implode(", ", $jsonFields) . ") as {$name}";
        }                                                                       
                                                                                
        return implode(", ", $fields);                                          
    }              

    /**
     * Builds the JOIN clauses for the query.
     *
     * @return string The JOIN clauses.
     */
    protected function buildJoins(): string
    {
        $schema = $this->schema;
        $joinClauses = [];

        foreach ($schema::joins() as $join) {
            $joinClauses[] = " {$join->type->value} JOIN
                {$join->to->table->name} {$join->to->table->alias} ON
                {$join->from->table->alias}.{$join->from->key} =
                {$join->to->table->alias}.{$join->to->key}";
        }

        foreach ($schema::relations() as $name => $relation) {
            $joinType = JoinType::LEFT; // Default join type is LEFT JOIN

            $joinClauses[] = " {$joinType->value}
                JOIN {$relation->to::table()->name} as $name
                ON {$relation->from->table->alias}.{$relation->from->key} =
            $name.{$relation->to::id()}
            ";
        }

        return implode(' ', $joinClauses);
    }

    /**
     * Builds the WHERE clause based on conditions in QueryParams.
     *
     * @return string The WHERE clause.
     */
    protected function buildWhere(): string
    {
        $whereClauses = [];
    
        // Function to process a condition and return the SQL clause
        $processCondition = function($condition, $index, $schema) use (&$whereClauses) {
            $field = $condition->field;
            $value = $condition->value;

            if ($value === null) {
                return;
            }

            $column = $schema::columns()[$field]->name;
            $alias = $schema::table()->alias;
            $comparisonOperator = $condition->operator->toSql();

            // Check if the operator is 'IN' and the value is an array
            if (is_array($value) && strtoupper($comparisonOperator) === 'IN') {
                $placeholders = [];
                foreach ($value as $i => $val) {
                    $placeholder = ":{$field}{$index}_{$i}";
                    $placeholders[] = $placeholder;
                    $this->params[$placeholder] = $val;  // Add the value to the params
                }
                $placeholderList = implode(', ', $placeholders);  // Create the placeholder list
                $clause = "$alias.$column $comparisonOperator ($placeholderList)";
            } else {
                // Standard placeholder for non-IN operations
                $placeholder = ":{$field}{$index}";
                $clause = "$alias.$column $comparisonOperator $placeholder";
                $this->params[$placeholder] = $value;
            }

            if (!empty($whereClauses)) {
                $logicalOperator = $condition->logicalOperator->value;
                $clause = "{$logicalOperator} {$clause}";
            }

            $whereClauses[] = $clause;
        };        
    
        // Process primary conditions
        foreach ($this->queryParams->conditions as $index => $condition) {
            $processCondition($condition, $index, $this->schema);
        }
    
        // Process includes conditions with relations
        foreach ($this->queryParams->includes as $includeIndex => $include) {
            $schema = $this->schema::relations()[$include->name]->schema;
    
            // Iterate over each condition within the includes array
            foreach ($include->conditions as $conditionIndex => $condition) {
                $processCondition($condition, "include{$includeIndex}_{$conditionIndex}", $schema);
            }
        }
    
        // Combine all where clauses
        $whereClauseStr = ' WHERE ' . implode(' ', $whereClauses);
        return !empty($whereClauses) ? $whereClauseStr : '';
    }

    /**
     * Builds the LIMIT and OFFSET clause for the query.
     *
     * @return string The LIMIT and OFFSET clause.
     */
    private function buildLimitOffset(): string
    {
        // if (is_null($this->queryParams->limit)) {
        //     return '';
        // }

        // $limitOffset = " LIMIT " . $this->queryParams->limit;
        $limitOffset = " LIMIT 50";

        if (!is_null($this->queryParams->offset)) {
            $limitOffset .= " OFFSET " . $this->queryParams->offset;
        }

        return $limitOffset;
    }
}
