<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\models\Units;
use LORIS\biobank\models\container\Types as ContainerTypes;

final class Type extends Model
{
    public function __construct(
        public readonly ?string $label,
        public readonly ?bool $freezeThaw,
        public readonly ?Units $units,
        public readonly ?ContainerTypes $containers,
        public readonly ?Types $parents,
        public readonly ?Protocols $protocols,
        private readonly ?int $id = null,
    ) {}
}
