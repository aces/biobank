<?php declare(strict_types=1);  

namespace LORIS\biobank;

use LORIS\Data\ProvisionerInstance;
use LORIS\biobank\Model;

abstract class ContainerDAO extends DAO
{
    protected \Database $db;
    protected bool $eagerLoading;
    protected QueryBuilder $queryBuilder;

    public function __construct(\Database $db) 
    {
        $this->queryBuilder = new QueryBuilder(ContainerSchema::eager());
        parent::construct($db);
    }
}
