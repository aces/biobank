<?php declare(strict_types=1);  

namespace LORIS\biobank\types;

class Time implements \JsonSerializable {
    private \DateTime $time;

    public function __construct(string $timeStr) {
        $this->time = new \DateTime($timeStr);
    }

    public function format(string $format): string {
        return $this->time->format($format);
    }

    public function jsonSerialize(): string {
        return $this->format('H:i'); // Time format
    }
}
