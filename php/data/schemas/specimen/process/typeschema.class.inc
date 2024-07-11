<?php

namespace LORIS\biobank\data\schemas\specimen\process;

use LORIS\biobank\data\AbstractSchema;
use LORIS\biobank\data\schema;

/**
 * Specimen Process Schema Descriptor Class
 * This class encapsulates the table definitions and provides
 * a map to key values to be used within the business layer.
 *
 * PHP Version 7.2
 *
 * @category   Schema
 * @package    Loris
 * @subpackage Biobank
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
class ProcessSchema extends AbstractSchema {
    protected function initialize(): void {
        // Define table and primary key
        $this->table = new schema\Table(
            name: 'biobank_specimen_process',
        );
        $this->id = new schema\Key('SpecimenProcessID');

        // Define columns
        $this->columns = [
            'label' => new schema\Column(
                name: 'Label',
                type: schema\ColumnType::VARCHAR
            )
        ];
    }
}
