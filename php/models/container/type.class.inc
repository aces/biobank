<?php declare(strict_types=1);

namespace LORIS\biobank\models\container;

use LORIS\biobank\Model;
use LORIS\biobank\models\container\type\Dimension;
use LORIS\biobank\models\container\type\Capacity;

class Type extends Model
{
    // TODO: I don't think dimension and capacity should be nullable, but my
    // query I think is messed up so it's temporary.
    public function __construct(
        public string $label,
        public string $brand,
        public string $productNumber,
        public bool $prime,
        public ?Dimension $dimension = null,
        public ?Capacity $capacity = null,
        private ?int $id = null,
    ) {}
}
