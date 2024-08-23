<?php declare(strict_types=1);   

namespace LORIS\biobank\models;                                                    

use LORIS\biobank\Model;
                                                                                   
final class Project extends Model                                                           
{                                                                                  
    public function __construct(                                                   
        public readonly \ProjectID $id,                                                           
        public readonly ?string $label,                                                      
    ) {}                                                                           
}   
