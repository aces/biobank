<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

class UnitSchema extends AbstractSchema
{
    public static function getTable(): Table
    {
        return new Table(name: 'biobank_unit');
    }

    public static function getId(): Key
    {
        return new Key('UnitID');
    }

    public static function getLabel(): string
    {
        return 'label';
    }

    public static function getColumns(): array
    {
        return [
            'label' => new Column(
                name: 'Label',
                type: ColumnType::VARCHAR
            ),
        ];
    }

    public static function getJoins(): array
    {
        return [];
    }

    public static function getRelations(): array                                
    {                                                                           
        return [];
    }
}
