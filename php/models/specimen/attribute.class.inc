<?php declare(strict_types=1); 

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\specimen\attribute;

final class Attribute extends Model
{
    public function __construct(
        public string $label,
        public Data $data
        private ?int $id = null,
    ) {}
}
