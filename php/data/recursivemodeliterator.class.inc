<?php declare(strict_types=1);

namespace LORIS\biobank\data;

use Loris\biobank\Factory;
use Loris\biobank\Model;

/**
 * Class RecursiveModelIterator
 *
 * Iterates over a set of models using a generator, providing recursive
 * iteration capabilities. This class works with JSON data and decodes it
 * into model instances using a factory. It is compatible with recursive
 * structures, making it suitable for handling hierarchical or nested data.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @implements \RecursiveIterator
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
class RecursiveModelIterator implements \RecursiveIterator
{
    /**
     * Stores the current row of data from the generator.
     *
     * @var array|null
     */    
    private ?array $currentRow = null;

    /**
     * Constructs the RecursiveModelIterator.
     *
     * @param \Generator $rows    The generator providing the rows of data.
     * @param Factory    $factory The factory used to create model instances.
     */    
    public function __construct(
        private \Generator $rows,
        private Factory $factory
    ) {
        $this->next(); // Initialize the first row
    }

    /**
     * Checks if the current element has children (i.e., if it is a nested
     * structure).
     *
     * @return bool True if the current element contains children, false
     *              otherwise.
     */    
    public function hasChildren(): bool
    {
        $data = json_decode($this->current()['json'], true);
        return is_array($data);
    }

    /**
     * Returns an iterator for the current element's children.
     *
     * Note: This assumes that the data contains a valid JSON array. If not,
     * it will generate an error when attempting to decode.
     *
     * @return \RecursiveIterator A new RecursiveModelIterator for the
     *                            children.
     */    
    public function getChildren(): \RecursiveIterator
    {
        $data = json_decode($this->current()['json'], true);
        return new self(new \ArrayIterator($data), $this->factory);
    }

    /**
     * Returns the current model instance after decoding its data.
     *
     * @return ?Model The current model instance, or null if the data cannot
     *                be decoded or is not valid.
     */    
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

    /**
     * Returns the current key.
     *
     * @return mixed|null The key, or null if no key is used.
     */    
    public function key(): mixed
    {
        return null; // Key can be removed if not necessary
    }

    /**
     * Does nothing, as the generator cannot be rewound.
     */    
    public function rewind(): void
    {
        // Do nothing, as we can't rewind a generator
    }

    /**
     * Advances the generator to the next element.
     */    
    public function next(): void
    {
        if ($this->rows->valid()) {
            $this->currentRow = $this->rows->current();
            $this->rows->next();
        } else {
            $this->currentRow = null;
        }
    }

    /**
     * Checks if the current row is valid.
     *
     * @return bool True if the current row is valid, false otherwise.
     */    
    public function valid(): bool
    {
        return $this->currentRow !== null;
    }    

    /**
     * Streams the models by yielding each valid instance.
     *
     * @return \Generator A generator that yields model instances.
     */    
    public function stream(): \Generator
    {
        while ($this->valid()) {
            yield $this->current();
            $this->next();
        }
    }
}

