<?php declare(strict_types=1);

namespace LORIS\biobank\models\container;

use LORIS\biobank\Model;
use LORIS\biobank\models\container\type\Dimension;
use LORIS\biobank\models\container\type\Capacity;

class Type extends Model
{
    public function __construct(
        public ?string $label,
        public ?string $brand,
        public ?string $productNumber,
        public ?bool $prime,
        public ?Dimension $dimension,
        public ?Capacity $capacity,
        public ?int $id,
    ) {}
}
