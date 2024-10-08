<?php

namespace LORIS\biobank\data\schemas\specimen\process;

use \LORIS\biobank\data\schema;
use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schemas\specimen\ProcessSchema;

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
class CollectionSchema extends ProcessSchema
{
    public static function model(): string                                          
    {                                                                              
        return \LORIS\biobank\models\specimen\process\Collection::class;                                
    }   
    
    public static function table(): schema\Table
    {
        return new schema\Table(name: 'biobank_specimen_collection');
    }

    public static function columns(): array
    {
        return parent::columns() + [
            'quantity' => new schema\Column(
                name: 'Quantity', 
                table: self::table()
            )
        ];
    }

    public static function relations(): array
    {
        return parent::relations() + [
            'unit' => new schema\Relation(
                from: new schema\TableKey(
                    table: self::table(),
                    key: 'UnitID'
                ),
                to: schemas\UnitSchema::class,
            )
        ];
    }
}
