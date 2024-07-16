<?php declare(strict_types=1);  

namespace LORIS\biobank\models;                                                 

use LORIS\biobank\Model;
                                                                                
class Center extends Model
{                                                                               
    public function __construct(                                                
        private int $id,                                                         
        public string $label,                                                 
    ) {}                                                                        
}  
