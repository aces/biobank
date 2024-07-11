<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;

class UnitSchema extends AbstractSchema
{
    protected function initialize(): void {

        // Define table and primary key
        $this->table = new Table(name: 'biobank_unit');
        $this->id = new Key('UnitID');

        // Define columns
        $this->columns = [
            'label' => new Column(
                name: 'Label',
                type: ColumnType::VARCHAR
            ),
        ];
    }
}
