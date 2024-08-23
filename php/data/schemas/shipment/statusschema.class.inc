<?php declare(strict_types=1);

namespace LORIS\biobank\data\schemas\shipment;

use \LORIS\biobank\data\schema\Table;
use \LORIS\biobank\data\schema\Column;
use \LORIS\biobank\data\schema\ColumnType;
use \LORIS\biobank\data\Schema;

/**
 * Shipment Status Schema Class
 *
 * This class defines the schema for the `shipment_status` table.
 *
 * PHP Version 7.2
 *
 * @category   Data
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */

class StatusSchema extends Schema
{
    protected function getModel(): string                                          
    {                                                                              
        return \LORIS\biobank\models\shipment\Status::class;                            
    }   
    
    protected function getTable(): Table {
        return new Table(name: 'shipment_status');
    }

    protected function getId(): string {
        return 'StatusID';
    }

    protected function getLabel(): string {
        return 'label';
    }

    protected function getColumns(): array {
        return [
            'label' => new Column(
                name: 'Label',
                table: $this->table,
            )
        ];
    }

    protected function getRelations(): array {
        return [];
    }

    protected function getJoins(): array {
        return [];
    }
}
