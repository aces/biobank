<?php declare(strict_types=1);

namespace LORIS\biobank\Models\Specimen\Attribute;

class Datatype extends Model
{
    public function __construct(
        private int $id,
        public string $label
    ) {}
}
