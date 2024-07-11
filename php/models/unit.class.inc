<?php declare(strict_types=1);  

namespace LORIS\biobank\models;                                                 
                                                                                
class Unit extends Model
{                                                                               
    public function __construct(                                                
        private int $id,                                                         
        public string $label,                                                 
    ) {}                                                                        
}  
