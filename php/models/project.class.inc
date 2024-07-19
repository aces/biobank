<?php declare(strict_types=1);   

namespace LORIS\biobank\models;                                                    

use LORIS\biobank\Model;
                                                                                   
final class Project extends Model                                                           
{                                                                                  
    public function __construct(                                                   
        public readonly string $label,                                                      
        private readonly ?int $id = null,                                                           
    ) {}                                                                           
}   
