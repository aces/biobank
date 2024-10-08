<?php declare(strict_types=1);   

namespace LORIS\biobank\models;                                                    

use LORIS\biobank\Model;
                                                                                   
final class Diagnosis extends Model                                                           
{                                                                                  
    public function __construct(                                                   
        public readonly ?string $abbreviation,                                                      
        public readonly ?string $label,                                                      
        public readonly ?int $id,                                                           
    ) {}                                                                           
}   
