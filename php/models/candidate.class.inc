<?php declare(strict_types=1);   

namespace LORIS\biobank\models;                                                    

use LORIS\biobank\Model;
                                                                                   
final class Candidate extends Model                                                           
{                                                                                  
    public function __construct(                                                   
        public readonly string $label,                                                      
        public readonly ?string $dob = null,
        public readonly ?string $sex = null,
        public readonly ?Sessions $sessions = null,
        public readonly ?Diagnoses $diagnoses = null,
        private readonly ?int $id = null,                                                           
    ) {}                                                                           
}   
