<?php declare(strict_types=1);  

namespace LORIS\biobank;

use LORIS\biobank\data\query\QueryParams;

class Controller {
    protected $service;

    public function __construct(Service $service, Factory $factory) {
        $this->service = $service;
        $this->factory = $factory;
    }

    // Abstract methods for handling requests
    public function get(QueryParams $params): array //Model[]
    {
        return $this->service->getInstances($params);
    }

    // Abstract methods for handling requests
    public function getByLabel($label): Model
    {
        return $this->service->getInstanceByLabel($label);
    }

    public function create(array $data): array
    {
        if (isset($data[0]) && is_array($data[0])) {
            $instances = $this->factory->from($instance);
            $this->service->makeInstance($instances);
        } else {
            $instance = $this->factory->fromList($data);
            return $this->service->makeInstance($instance);
        }
    }

    public function update(array $data): array {
        if (isset($data[0]) && is_array($data[0])) {
            $instances = $this->factory->from($data);
            $this->service->updateInstances($instances);
        } else {
            $instance = $this->factory->fromList($data);
            return $this->service->updateInstance($instance);
        }
    }
}
