<?php declare(strict_types=1);  

namespace LORIS\biobank\data;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\schema\LoadingStrategyTrait;

abstract class AbstractSchema implements Schema {
    use LoadingStrategyTrait;

    abstract public static function getTable(): Table;
    abstract public static function getId(): Key;
    abstract public static function getLabel(): string;
    abstract public static function getColumns(): array;
    abstract public static function getRelations(): array;
    abstract public static function getJoins(): array;

    public function getFields(): array {
        $fields = array_keys(static::getColumns());
        if ($this->loadingStrategy === LoadingStrategy::EAGER) {
            $fields = array_merge($fields, array_keys(static::getRelations()));
        }
        return $fields;
    }

    public static function getIdKey(): TableKey {
        return new TableKey(
            table: static::getTable(),
            key: static::getId()
        );
    }
}
