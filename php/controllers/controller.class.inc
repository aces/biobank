<?php declare(strict_types=1);  

abstract class Service {

     private $loris;
     private $db;
     private $user;
     protected DAO $dao;

    public function __construct(\LORIS\LorisInstance $loris, \User $user) {

        $this->loris = $loris;
        $this->db    = $loris->getDatabaseConnection();
        $this->user  = $user;
        $this->dao   = $this->_getDataProvisioner();
    }

    /**
     * Common method to get an instance by id. Expected to be overridden or
     * utilized as is by child classes.
     *
     * @param mixed $id The identifier of the entity.
     * @return mixed
     */
    public function getById($id) {
        return $this->dao->findById($id);
    }

    /**
     * Common method to save or update an entity. This method can be overridden
     * to include business logic specific to the service.
     *
     * @param mixed $entity The entity to save or update.
     * @return mixed
     */
    public function save($entity) {
        return $this->dao->save($entity);
    }

    public function update($entity) {
        return $this->dao->update($entity);
    }

    /**
     * Common method to delete an entity.
     *
     * @param mixed $entity The entity to delete.
     * @return void
     */
    // public function delete($entity) {
    //     return $this->dao->delete($entity);
    // }

    /**
     * Method that all derived services must implement to handle their specific
     * tasks.
     */
    // abstract protected function executeSpecificTask();

    /**
     * Validate the entity as per the business rules. This should be implemented
     * by each service to handle specific validation rules.
     *
     * @param mixed $entity The entity to validate.
     * @return bool True if validation passes, false otherwise.
     */
    abstract protected function validate($entity);
}
