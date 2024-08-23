<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\shipment;

use \LORIS\biobank\data\schemas;
use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\TableKey;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\schema\Relation;
use \LORIS\biobank\data\schema\RelationType;
use \LORIS\biobank\data\schema\Join;
use \LORIS\biobank\data\schema\LoadingStrategy;
use \LORIS\biobank\data\Schema;

/**
 * Log Schema Class
 * 
 * This class defines the schema for the `shipment_log` table.
 * 
 * PHP Version 7.2
 * 
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class LogSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\shipment\Log::class;                                 
    }  

    protected function getTable(): Table {
        return new Table(name: 'shipment_log');
    }

    protected function getId(): string {
        return 'ShipmentID';
    }

    protected function getLabel(): string {
        return '';
    }

    protected function getColumns(): array {
        return [
            'temperature' => new Column(
                name: 'Temperature',
                table: $this->table,
            ),
            'date' => new Column(
                name: 'Date',
                table: $this->table,
            ),
            'time' => new Column(
                name: 'Time',
                table: $this->table,
            ),
            'comments' => new Column(
                name: 'Comments',
                table: $this->table,
            ),
        ];
    }

    protected function getRelations(): array {
        return [
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'CenterID'
                ),
                to: schemas\CenterSchema::class,
            ),
            'status' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'StatusID'
                ),
                to: StatusSchema::class,
            ),
            'user' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: 'UserID'
                ),
                to: schemas\UserSchema::class,
            ),
        ];
    }

    protected function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
