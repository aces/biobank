<?php declare(strict_types=1);  

namespace LORIS\biobank;

use LORIS\Data\ProvisionerInstance;
use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\Schema;
use LORIS\biobank\data\query\QueryParams;
use LORIS\biobank\data\query\QueryBuilder;

abstract class Service
{
    public function __construct(
        protected ModelProvisioner $provisioner,
        protected ?Validator $validator = null
    ) {}

    abstract protected function handlePostSave(Model $instance): void;

    protected function applyDefaultParams(QueryParams $params): QueryParams {
        return $params;
    }

    public function get(QueryParams $queryparams): ModelProvisioner
    {
        $params = $this->applyDefaultParams($queryparams);
        return $this->provisioner->with(...$params->includes)->get($queryparams);
    }

    public function getLabels(QueryParams $params): array
    {
        if (count($params->fields) !== 1 || $params->fields[0] !== 'label') {
            throw new InvalidArgumentException("This method only retrieves labels");
        }

        return $this->dao->findLabels($params);
    }


    protected function countInstances(QueryParams $params): int
    {
        $params->countOnly(true);
        // TODO: potentially merge params here...
        return $this->dao->find($params);
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
        // XXX this is here so that I can omit validators for current services
        // that live in the biobank but shouldn't long term (i.e. centers,
        // projects, candidates, etc.)
        if ($this->validator === null) {
            throw new \LogicException("Validator is required for saving instances.");
        }

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
