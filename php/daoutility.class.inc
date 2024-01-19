<?php

namespace LORIS\biobank;

class DAOUtility {

    /**                                                                         
     * Builds the WHERE clause for a SQL query based on given conditions.       
     *                                                                          
     * @param array $conditions Conditions for the WHERE clause.                
     * @return array Returns an array with the WHERE clause and parameters.     
     */                                                                         
    public static function buildWhereClause(array $conditions): array {               
        $whereClause = [];                                                      
        $queryParams = [];                                                      

                                                                                
        foreach ($conditions as $index => $condition) {                                   
            $column = $condition['column'];
            $value = $condition['value'];

            // Set default values if not provided                               
            $comparison_operator = $condition['comparison_operator'] ?? '=';    
            $logical_operator = $condition['logical_operator'] ?? 'AND';        

            if ($value === null) {
                continue;
            }

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

}
