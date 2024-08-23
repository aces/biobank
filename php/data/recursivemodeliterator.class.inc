<?php declare(strict_types=1);

namespace LORIS\biobank\data;

use Loris\biobank\Factory;
use Loris\biobank\Model;

class RecursiveModelIterator implements \RecursiveIterator
{
    private ?array $currentRow = null;

    public function __construct(
        private \Generator $rows,
        private Factory $factory
    ) {
        $this->next(); // Initialize the first row
    }

    public function hasChildren(): bool
    {
        $data = json_decode($this->current()['json'], true);
        return is_array($data);
    }

    public function getChildren(): \RecursiveIterator
    {
        $data = json_decode($this->current()['json'], true);
        return new self(new \ArrayIterator($data), $this->factory);
    }

    public function current(): ?Model
    {
        $data = json_decode($this->currentRow['json'], true);
        
        //error_log(print_r($data, true));
        if (json_last_error() === JSON_ERROR_NONE && is_array($data)) {
            return $this->factory->create($data);
        } else {
            error_log("JSON decode error or data is not an array: " . json_last_error_msg());
            return null; // Ensure that this matches your Model return type
        }
    }    

    public function key(): mixed
    {
        return null; // Key can be removed if not necessary
    }

    public function rewind(): void
    {
        // Do nothing, as we can't rewind a generator
    }

    public function next(): void
    {
        if ($this->rows->valid()) {
            $this->currentRow = $this->rows->current();
            $this->rows->next();
        } else {
            $this->currentRow = null;
        }
    }

    public function valid(): bool
    {
        return $this->currentRow !== null;
    }    

    public function stream(): \Generator
    {
        while ($this->valid()) {
            yield $this->current();
            $this->next();
        }
    }
}

