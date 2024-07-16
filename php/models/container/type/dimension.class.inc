<?php

namespace LORIS\biobank\models\container\type;

use LORIS\biobank\Model;

class Dimension extends Model
{
    public function __construct(
        public ?int $x = null,
        public ?bool $xNumerical = null,
        public ?int $y = null,
        public ?bool $yNumerical = null,
        public ?int $z = null,
        public ?bool $zNumerical = null,
        private ?int $id = null,
    ) {}
}
