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

    public readonly Table $table;
    public readonly Key $id;
    public readonly TableKey $idKey;
    public readonly string $label;
    public readonly array $columns;
    public readonly array $joins;
    public readonly array $relations;

    public function __construct(
        public readonly string $name,
        protected LoadingStrategy $strategy,
    ) {

        $this->table = $this->getTable();
        $this->id = $this->getId();
        $this->label = $this->getLabel();
        $this->idKey = new TableKey(table: $this->getTable(), key: $this->getId());
        $this->columns = $this->getColumns();
        $this->joins = $this->getJoins();
        $this->relations = $this->buildRelations();
    }

    protected function buildRelations(): array
    {
        if ($this->isLazy()) {
            return [];
        }

        $relations = [];
        foreach ($this->getRelations() as $field => $relation) {
            $relation->buildSchema($this->name, $field);
            $relations[$field] = $relation;
        }

        return $relations;
    }

    abstract protected function getTable(): Table;
    abstract protected function getId(): Key;
    abstract protected function getLabel(): string;
    abstract protected function getColumns(): array;
    abstract protected function getJoins(): array;
    abstract protected function getRelations(): array;
}
