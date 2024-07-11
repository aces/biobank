<?php declare(strict_types=1);

namespace LORIS\biobank\Models\Specimens;

use LORIS\biobank\types\Date;
use LORIS\biobank\types\Time;

class Process extends Model
{
    public function __construct(
        private int $id,
        public Protocol $protocol,
        public Center $center,
        public Examiner $examiner,
        public Date $date,
        public Time $time,
        public string $comments,
        public Data $data
    ) {}
}
