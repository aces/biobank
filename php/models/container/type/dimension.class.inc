<?php

namespace LORIS\biobank\models\container\type;

use LORIS\biobank\Model;

class Dimension extends Model
{
    public function __construct(
        public ?int $x,
        public ?bool $xNum,
        public ?int $y,
        public ?bool $yNum,
        public ?int $z,
        public ?bool $zNum,
        public ?int $id,
    ) {}
}
