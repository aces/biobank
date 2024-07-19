<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen\attribute;

use LORIS\biobank\Model;

final class Data extends Model
{
    public function __construct(
        public readonly string $label
        private readonly ?int $id = null,
    ) {}
}
