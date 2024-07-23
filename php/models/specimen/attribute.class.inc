<?php declare(strict_types=1); 

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\models\specimen\attribute\Datatype;

final class Attribute extends Model
{
    public function __construct(
        public string $label,
        public DataType $datatype,
        private ?int $id = null,
    ) {}
}
