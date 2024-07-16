<?php declare(strict_types=1);

namespace LORIS\biobank\data\schema;

class Table                                                                       
{                                                                                  
    public function __construct(                                                   
        public string $name,                                                       
        public ?string $alias = null
    ) {
        $this->alias = $alias ?? $name;
    }                                                                           
}  
