namespace LORIS\biobank\Models;

class Models implements \IteratorAggregate, \Countable, \JsonSerializable
{
    private array $models;

    public function __construct(array $models)
    {
        $this->models = $models;
    }

    public static function fromArray(array $data, string $modelClass): Models
    {
        $models = array_map([$modelClass, 'fromArray'], $data);
        return new Models($models);
    }

    public function getIterator(): \ArrayIterator
    {
        return new \ArrayIterator($this->models);
    }

    public function count(): int
    {
        return count($this->models);
    }

    public function jsonSerialize(): array
    {
        return array_map(function($model) {
            return $model->toArray();
        }, $this->models);
    }

    public function toArray(): array
    {
        return $this->jsonSerialize();
    }
}
