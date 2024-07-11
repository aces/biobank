<?php declare(strict_types=1);

namespace LORIS\biobank\models\container;

use LORIS\biobank\Model;

class Type extends Model
{
    public function __construct(
        private int $id,
        public string $label,
        public string $brand,
        public string $productNumber,
        public bool $primary,
        public Capacity $capacity,
        public Dimension $dimension
    ) {}
}
