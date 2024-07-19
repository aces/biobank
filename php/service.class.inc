<?php declare(strict_types=1);  

namespace LORIS\biobank;

use LORIS\biobank\data\DAO;
use LORIS\biobank\data\query\QueryParams;

abstract class Service {

    protected DAO $dao;
    private Validator $validator;

    // abstract public function mapInstances(array $instances): array;
    abstract protected function handlePostSave(Model $instance): void;
    // abstract protected function map(array $instanceArray);

    public function __construct(DAO $dao, Validator $validator) {

        $this->dao = $dao;
        $this->validator = $validator;
    }

    public function getInstances(QueryParams $params): array //Model[]
    {
        return $this->dao->find($params);
    }

    public function getInstanceByLabel(string $label) : Model
    {
        $instance = $this->dao->findByLabel($label);
        if (!$instance) {
            throw new \Exception("Instance not found");
        }
        return $instance;
    }

    public function save(array $instances): array {
        $this->dao->begin();

        try {
            $validation = $this->validator->validateInstances($instances);
            if ($validation->hasErrors()) {
                throw new BadRequest(json_encode($validation->getErrors()));
            }

            $savedInstances = [];
            foreach ($instances as $instance) {
                $savedInstance = $this->dao->save($instance);
                $this->handlePostSave($savedInstance);
                $savedInstances[] = $savedInstance;
            }

            $this->dao->commit();
            return $savedInstances;
        } catch (\Exception $e) {
            $this->dao->rollback();
            throw $e;
        }
    }

    public function delete(string $id) : void
    {
        $this->dao->begin();
        try {
            $instance = $this->dao->find($id);
            if (!$instance) {
                throw new Exception("Instance not found");
            }
            $this->dao->delete($id);
            $this->dao->commit();
        } catch (Exception $e) {
            $this->dao->rollback();
            throw $e;
        }
    }
}
