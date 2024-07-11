<?php declare(strict_types=1);

namespace LORIS\biobank\models\container\type;

use LORIS\biobank\Model;
use LORIS\biobank\models\Unit

class Capacity extends Model
{
    public function __construct(
        private int $id,
        public float $quantity,
        public Unit $unit
    ) {}
}
