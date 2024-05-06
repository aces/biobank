<?php declare(strict_types=1);  

abstract class DAO {
    protected $db;

    public function __construct(\Database $db) {
        $this->db = $db;
    }

    abstract protected function _getSQLFromInstance($object);
    abstract public function findById($id);
    abstract private function getFromSQL(array $data) : \LORIS\Data\DataInstance
    abstract private function getSQL(\LORIS\Data\DataInstnace $instance) : array 

    protected function select(
        ?array $conditions = [],
        ?int $limit = null,
        int $offset = 0
    ) : \LORIS\Data\DataInstance[]

    protected static function buildQuery(
        string $query,
        array $conditions = [],
        ?string $groupBy = null,
        ?int $limit = null,
        ?int $offset = 0
    ): array {
        $queryParams = [];

        // Handle conditions
        if (!empty($conditions)) {
            $whereData = self::buildWhereClause($conditions);
            $query .= $whereData['clause'];
            $queryParams = array_merge($queryParams, $whereData['params']);
        }

        // Handle grouping
        if (!is_null($groupBy)) {
            $query .= " GROUP BY " . $groupBy;
        }

        // Handle pagination
        if (!is_null($limit)) {
            // Directly incorporate limit and offset into the query string
            $query .= " LIMIT " . (int)$limit . " OFFSET " . (int)$offset;
            // $queryParams[':limit'] = (int) $limit;
            // $queryParams[':offset'] = (int) $offset;
        }

        return ['query' => $query, 'params' => $queryParams];
    }

    /**
     * Builds the WHERE clause for a SQL query based on given conditions.
     *
     * @param array $conditions Conditions for the WHERE clause.
     * @return array Returns an array with the WHERE clause and parameters.
     */
    protected static function buildWhereClause(array $conditions): array {
        $whereClause = [];
        $queryParams = [];

        foreach ($conditions as $index => $condition) {
            $column = $condition['column'];
            $value = $condition['value'];

            if ($value === null) {
                continue;
            }

            // Ensure column names are safe to use directly in the SQL statement
            if (!preg_match('/^([a-zA-Z0-9_]+\.?[a-zA-Z0-9_]+)$/', $column)) {
                throw new \Exception("Invalid column name: " . $column);
            }

            // Set default values if not provided
            $comparison_operator = $condition['comparison_operator'] ?? '=';
            $logical_operator = $condition['logical_operator'] ?? 'AND';

            $placeholder = ":param{$index}";

            // The first condition does not prepend a logical operator
            $clause = count($whereClause) > 0 ? " $logical_operator " : ' WHERE ';
            $clause .= "$column $comparison_operator $placeholder";
            $whereClause[] = $clause;
            $queryParams[$placeholder] = $value;
        }

        $whereClauseStr = implode('', $whereClause);

        return ['clause' => $whereClauseStr, 'params' => $queryParams];
    }

    protected static function toArray(?string $string) : array {
        return !empty($string) ? explode(',', $string) : [];
    }

    protected function getIdbyLabel(string $value, string $label, string $column, string $table) {
        $query = "SELECT $column FROM $table WHERE $label = :i";
        return $this->db->pselectOne($query, ['i' => $value]);
    }
}

//    private function createCondition(
//        string $column,
//        $value,
//        string $comparison_operator = '=',
//        string $logical_operator = 'AND'): array
//    {
//        return [
//            'column' => $column,
//            'value' => $value,
//            'comparison_operator' => $comparison_operator,
//            'logical_operator' => $logical_operator
//        ];
//    }
