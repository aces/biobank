<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\specimen\Type;
use LORIS\biobank\specimen\Process;

final class Protocol extends Model
{
    public function __construct(
        public readonly string $label,
        public readonly Type $type,
        public readonly Process $process
        private readonly ?int $id = null,
    ) {}
}
