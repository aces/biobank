<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\models\specimen\Type;
use LORIS\biobank\models\specimen\process;

final class Protocol extends Model
{
    public function __construct(
        public readonly ?string $label,
        public readonly ?Type $type = null,
        public readonly ?process\Type $process = null,
        private readonly ?int $id = null,
    ) {}
}
