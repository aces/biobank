<?php declare(strict_types=1);   

namespace LORIS\biobank\models;                                                    

use LORIS\biobank\Model;
                                                                                   
final class Candidate extends Model                                                           
{                                                                                  
    public function __construct(                                                   
        public readonly ?string $label,                                                      
        public readonly ?string $dob,
        public readonly ?string $sex,
        public readonly ?Sessions $sessions,
        public readonly ?Diagnoses $diagnoses,
        private readonly ?int $id,                                                           
    ) {}                                                                           
}   
