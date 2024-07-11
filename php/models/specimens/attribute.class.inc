<?php declare(strict_types=1); 

namespace LORIS\biobank\Models\Specimen;

class Attribute extends Model
{
    public function __construct(
        private int $id,
        public string $label,
        public DataType $datatype
    ) {}
}
