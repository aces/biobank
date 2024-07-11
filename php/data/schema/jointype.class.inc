<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

enum JoinType: string                                                              
{                                                                                  
    case LEFT = 'LEFT';                                                            
    case RIGHT = 'RIGHT';                                                          
    case INNER = 'INNER';                                                          
    case OUTER = 'OUTER';                                                          
}    
