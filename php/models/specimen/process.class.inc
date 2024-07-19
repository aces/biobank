<?php declare(strict_types=1);

namespace LORIS\biobank\models\specimen;

use LORIS\biobank\Model;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\Examiner;
use LORIS\biobank\models\specimne\attribute\Data;
use LORIS\biobank\types\Date;
use LORIS\biobank\types\Time;

abstract class Process extends Model
{
    public function __construct(
        public Protocol $protocol,
        public Center $center,
        public Examiner $examiner,
        public Date $date,
        public Time $time,
        public string $comments,
        public Data $data
        private ?int $id = null,
    ) {}
}
