<?php

class Date implements JsonSerializable {
    private \DateTime $date;

    public function __construct(string $dateStr) {
        $this->date = new \DateTime($dateStr);
    }

    public function format(string $format): string {
        return $this->date->format($format);
    }

    public function jsonSerialize(): string {
        return $this->format('Y-m-d'); // Date format
    }
}
