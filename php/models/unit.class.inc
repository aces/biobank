<?php declare(strict_types=1);  

namespace LORIS\biobank\models;                                                 

use LORIS\biobank\Model;
                                                                                
class Unit extends Model
{                                                                               
    public function __construct(                                                
        public string $label,                                                 
        private ?int $id = null,                                                         
    ) {}                                                                        
}  
