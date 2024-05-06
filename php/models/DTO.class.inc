<?php declare(strict_types=1);

abstract class DTO {

     function __construct(array $data)
     {
         $this->fromArray($data);
     }

    /**
     * Populates the object properties from an array.
     *
     * @param array $data Data to be used to populate the object properties.
     *
     * @return self
     */
    protected function fromArray(array $data): self {
        foreach ($data as $key => $value) {
            $setter = 'set' . ucfirst($key);
            $this->set($setter, $value);
        }
        return $this;
    }

    /**
     * Generic setter function to call setter methods based on provided setter name and data.
     *
     * @param string $setter Setter method name.
     * @param mixed  $data   Data to set.
     *
     * @return void
     */
    protected function set(string $setter, $data): void {
        if (is_callable([$this, $setter])) {
            $this->{$setter}($data);
        }
    }

    /**
     * Converts the current object instance into an array.
     *
     * @return array
     */
    public function toArray(): array {
        return $this->jsonSerialize();
    }

    /**
     * Should be implemented by each subclass to convert the object to an array.
     *
     * @return array
     */
    public function jsonSerialize() {
        $data = [];
        $reflect = new ReflectionClass($this);
        $props   = $reflect->getProperties(ReflectionProperty::IS_PRIVATE);
    
        foreach ($props as $prop) {
            $prop->setAccessible(true); // Make private property accessible
            $key = $prop->getName();
            $value = $prop->getValue($this);
    
            // Check if there's a getter method for the property
            $getter = 'get' . ucfirst($key);
            if (method_exists($this, $getter)) {
                $value = $this->{$getter}(); // Use getter method if available
            }
    
            // Convert objects to suitable representations
            if ($value instanceof DateTime) {
                $value = $value->format('Y-m-d');
            }
    
            $data[$key] = $value;
        }
    
        return $data;
    }

   /**
    * Returns a JSON encoded string of the state of the Container
    *
    * @return string JSON encoded string of Container data
    */
   public function toJSON() : string
   {
       $json = json_encode($this);
       if (json_last_error() !== JSON_ERROR_NONE) {
           // Handle specific error, log it or throw an exception
           throw new \JsonException('Failed to serialize object to JSON: ' . json_last_error_msg());
       }
       return $json;       
   }

}
