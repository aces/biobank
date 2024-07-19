<?php

namespace LORIS\biobank\data\schemas\specimen\process;

use LORIS\biobank\data\schema;
use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schemas\specimen\ProcessSchema;

/**
 * Specimen Collection Schema Descriptor Class
 * This class encapsulates the table definitions for specimen collection 
 * and provides a map to key values to be used within the business layer.
 * 
 * PHP Version 7.2
 * 
 * @category   Schema
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class CollectionSchema extends ProcessSchema {
    protected function getTable(): schema\Table
    {
        return new schema\Table(name: 'biobank_specimen_collection');
    }

    protected function getColumns(): array
    {
        return parent::getColumns() + [
            'quantity' => new schema\Column(
                name: 'Quantity', 
                type: schema\ColumnType::DECIMAL
            )
        ];
    }

    protected function getRelations(): array
    {
        return parent::getRelations() + [
            'unit' => new schema\Relation(
                from: new schema\TableKey(
                    table: $this->table,
                    key: new schema\Key('UnitID')
                ),
                to: schemas\UnitSchema::class,
                strategy: schema\LoadingStrategy::LAZY
            )
        ];
    }
}
