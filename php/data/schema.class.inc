<?php declare(strict_types=1);  

namespace LORIS\biobank\data;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\schema\LoadingStrategyTrait;

abstract class Schema
{
    use LoadingStrategyTrait;

    public readonly string $model;
    public readonly Table $table;
    public readonly string $id;
    public readonly TableKey $idKey;
    public readonly string $label;
    public readonly array $columns;
    public readonly array $joins;
    public readonly array $relations;

    public function __construct(
        public readonly string $name,
        // protected LoadingStrategy $strategy,
    ) {
        $this->model = $this->getModel();
        $this->table = $this->getTable();
        $this->id = $this->getId();
        $this->label = $this->getLabel();
        $this->idKey = new TableKey(table: $this->getTable(), key: $this->getId());
        $this->joins = $this->getJoins();
        $this->relations = $this->getRelations();
        $this->columns = $this->getColumns();
    }

    abstract protected function getModel(): string;
    abstract protected function getTable(): Table;
    abstract protected function getId(): string;
    abstract protected function getLabel(): string;
    abstract protected function getColumns(): array;
    abstract protected function getJoins(): array;
    abstract protected function getRelations(): array;
}
