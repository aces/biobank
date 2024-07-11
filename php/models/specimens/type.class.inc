<?php declare(strict_types=1);

namespace LORIS\biobank\Specimen;

class Type extends Model
{
    public function __construct(
        private int $id,
        public string $label,
        public ?bool $freezeThaw
    ) {}
}
