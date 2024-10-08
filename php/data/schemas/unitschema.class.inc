<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use \LORIS\biobank\data\Schema;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;

final class UnitSchema extends Schema
{

    public static function model(): string
    {
        return \LORIS\biobank\models\Unit::class;
    }

    public static function table(): Table
    {
        return new Table(name: 'biobank_unit');
    }

    public static function id(): string
    {
        return 'UnitID';
    }

    public static function label(): string
    {
        return 'label';
    }

    public static function columns(): array
    {
        return [
            'label' => new Column(
                name: 'Label',
                table: self::table()
            ),
        ];
    }

    public static function joins(): array
    {
        return [];
    }

    public static function relations(): array                                
    {                                                                           
        return [];
    }
}
