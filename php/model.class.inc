<?php declare(strict_types=1);

namespace LORIS\biobank;

abstract class Model implements \LORIS\Data\DataInstance
{
    protected array $setProperties = [];

    /**
     * Converts the current object instance into an array, including the ID.
     *
     * @return array
     */
    public function toArray(): array
    {
        return array_merge(['id' => $this->id], $this->extract());
    }

    /**
     * Serializes the object to a JSON-serializable array.
     *
     * @return array
     */
    public function jsonSerialize(): array {
        return array_merge($this->extract(), $this->toJSON());
    }

    /**
     * Returns additional properties to be included in JSON serialization.
     * Child classes should override this method to add extra properties.
     *
     * @return array
     */
    protected function toJSON(): array {
        return [];
    }

    /**
     * Extracts properties from the current object instance into an array.
     *
     * @param bool $includeId Whether to include the ID in the extracted array.
     * @return array
     */
    protected function extract(): array
    {
        $data = [];
        $reflect = new ReflectionClass($this);
        $props = $reflect->getProperties(ReflectionProperty::IS_PUBLIC);
    
        foreach ($props as $prop) {
            $key = $prop->getName();
            $value = $prop->getValue($this);

            $data[$key] = $value;
        }
    
        return $data;
    }
}
