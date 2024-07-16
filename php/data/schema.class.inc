<?php

namespace LORIS\biobank\data;

use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\TableKey;

interface Schema {
    public readonly Table $table;
    public readonly Key $id;
    public readonly TableKey $idKey;
    public readonly string $label;
    public readonly array $columns;
    public readonly array $joins;
    public readonly array $relations;

    public static function getTable(): Table;
    public static function getId(): Key;
    public static function getIdKey(): TableKey;
    public static function getLabel(): string;
    public static function getColumns(): array;
    public static function getJoins(): array;
    public static function getRelations(): array;
}
