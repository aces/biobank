<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\Examiner;
use LORIS\biobank\types\Date;
use LORIS\biobank\types\Time;

abstract class Process extends Model
{
    public function __construct(
        private ?int $id = null,
        public ?Center $center = null,
        public ?Examiner $examiner = null,
        public ?Date $date = null,
        public ?Time $time = null,
        public ?string $comments = null,
        public ?array $data = null,
        public ?Protocol $protocol = null,
    ) {}
}
