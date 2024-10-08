<?php declare(strict_types=1);

namespace LORIS\biobank\data\query;

/**
 * Class QueryParams
 *
 * Represents the parameters used for building a query, including fields, conditions,
 * includes, limits, offsets, and order-by clauses. This class is mutable and allows
 * chaining of methods to build the parameters.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public License
 */
class QueryParams
{
    /**
     * Constructs a QueryParams instance with optional parameters.
     *
     * @param ?array $fields           The fields to be selected.
     * @param ?array $conditions       The conditions to be applied.
     * @param ?array $excludeConditions The conditions to be excluded.
     * @param ?array $includes         The related models to include.
     * @param ?int   $limit            The limit of results to return.
     * @param ?int   $offset           The offset for the results.
     * @param ?array $orderBy          The order-by fields for the query.
     * @param bool   $countOnly        Whether the query should only return a count.
     */    
    public function __construct(
        public ?array $fields = [],
        public ?array $conditions = [], //Condition[]
        public ?array $excludeConditions = [],
        public ?array $includes = [],
        public ?int $limit = null,
        public ?int $offset = null,
        public ?array $orderBy = null,
        public bool $countOnly = false,
    ) {}

    /**
     * Sets the fields to be selected in the query.
     *
     * @param string ...$fields The fields to select.
     * @return self
     */    
    public function select(string ...$fields): self
    {
        $this->fields = $fields;
        return $this;
    }

    /**
     * Adds conditions to the query.
     *
     * @param Condition ...$conditions The conditions to apply.
     * @return self
     */    
    public function where(Condition ...$conditions): self
    {
        foreach ($conditions as $condition) {
            $this->conditions[] = $condition;
        }
        return $this;
    }

    /**
     * Includes related models in the query.
     *
     * @param With ...$includes The related models to include.
     * @return self
     */    
    public function include(With ...$includes): self
    {
        $this->includes = array_merge($this->includes, $includes);
        return $this;
    }

    /**
     * Sets the group-by clause for the query.
     *
     * @param string $groupBy The field to group by.
     * @return self
     */    
    public function groupBy(string $groupBy): self
    {
        $this->groupBy = $groupBy;
        return $this;
    }

    /**
     * Sets the limit for the query results.
     *
     * @param ?int $limit The maximum number of results.
     * @return self
     */    
    public function limit(?int $limit): self
    {
        $this->limit = $limit;
        return $this;
    }

    /**
     * Sets the offset for the query results.
     *
     * @param ?int $offset The offset for the results.
     * @return self
     */    
    public function offset(?int $offset): self
    {
        $this->offset = $offset;
        return $this;
    }

    /**
     * Sets whether the query should only return a count of the results.
     *
     * @param bool $countOnly Whether to return only a count.
     * @return self
     */    
    public function countOnly(bool $countOnly): self
    {
        $this->countOnly = $countOnly;
        return $this;
    }

    /**
     * Checks if any query parameters have been set.
     *
     * @return bool True if any parameters are set, false otherwise.
     */
    public function hasAnyParamsSet(): bool
    {
        return $this->countOnly ||
               !empty($this->conditions) ||
               !empty($this->fields) ||
               !empty($this->includes) ||  // If you consider includes as a meaningful param
               isset($this->limit) ||
               isset($this->offset);
    }
}
