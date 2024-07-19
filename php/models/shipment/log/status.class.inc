<?php declare(strict_types=1);  

namespace LORIS\biobank\models\shipment\log;                                                 
                                                                                
class Status extends Model
{                                                                               
    public function __construct(                                                
        public string $label,                                                 
        public ?int $id = null,                                                         
    ) {}                                                                        
}  
