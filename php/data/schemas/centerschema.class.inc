<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas;

use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\AbstractSchema;


/**
 * Center Schema Class
 * 
 * This class defines the schema for the `psc` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class CenterSchema extends AbstractSchema
{
    public static function getTable(): Table {
        return new Table(name: 'psc');
    }

    public static function getId(): Key {
        return new Key(name: 'CenterID');
    }

    public static function getLabel(): string {
        return 'label';
    }

    public static function getColumns(): array {
        return [
            'label' => new Column(
                name: 'Name',
                type: ColumnType::VARCHAR
            ),
        ];
    }

    public static function getRelations(): array {
        return []; // Add relation definitions if needed
    }

    public static function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
