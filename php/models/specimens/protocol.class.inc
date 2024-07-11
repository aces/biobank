<?php declare(strict_types=1);

namespace LORIS\biobank\Specimen;

use LORIS\biobank\specimen\Type;
use LORIS\biobank\specimen\Process;

class Protocol extends Model
{
    public function __construct(
        private int $id,
        public string $label,
        public Type $type,
        public Process $process
    ) {}
}
