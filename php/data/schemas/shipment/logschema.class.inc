<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\shipment;

use LORIS\biobank\data\schemas;
use LORIS\biobank\data\schema\Table;
use LORIS\biobank\data\schema\Key;
use LORIS\biobank\data\schema\TableKey;
use LORIS\biobank\data\schema\Column;
use LORIS\biobank\data\schema\ColumnType;
use LORIS\biobank\data\schema\Relation;
use LORIS\biobank\data\schema\Join;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\data\Schema;

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
    protected function getTable(): Table {
        return new Table(name: 'shipment_log');
    }

    protected function getId(): Key {
        return new Key(name: 'ShipmentID');
    }

    protected function getLabel(): string {
        return '';
    }

    protected function getColumns(): array {
        return [
            'temperature' => new Column(
                name: 'Temperature',
                type: ColumnType::DECIMAL
            ),
            'date' => new Column(
                name: 'Date',
                type: ColumnType::DATE
            ),
            'time' => new Column(
                name: 'Time',
                type: ColumnType::TIME
            ),
            'comments' => new Column(
                name: 'Comments',
                type: ColumnType::VARCHAR
            ),
        ];
    }

    protected function getRelations(): array {
        return [
            'center' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key(name: 'CenterID')
                ),
                to: schemas\CenterSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
            'status' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key(name: 'StatusID')
                ),
                to: StatusSchema::class,
                strategy: LoadingStrategy::EAGER
            ),
            'user' => new Relation(
                from: new TableKey(
                    table: $this->table,
                    key: new Key(name: 'UserID')
                ),
                to: schemas\UserSchema::class,
                strategy: LoadingStrategy::LAZY
            ),
        ];
    }

    protected function getJoins(): array {
        return []; // Add join definitions if needed
    }
}
