<?php

namespace LORIS\biobank\models\container\type;

use LORIS\biobank\Model;

class Dimension extends Model
{
    public function __construct(
        private int $id,
        public int $x,
        public bool $xNumerical,
        public int $y,
        public bool $yNumerical,
        public int $z,
        public bool $zNumerical
    ) {}
}
