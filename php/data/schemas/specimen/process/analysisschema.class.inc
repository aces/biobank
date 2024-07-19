<?php

namespace LORIS\biobank\data\schemas\specimen\process;

use LORIS\biobank\data\schema;
use LORIS\biobank\data\schemas\specimen\ProcessSchema;

/**
 * Specimen Analysis Schema Descriptor Class
 * This class encapsulates the table definitions for specimen analysis 
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
class AnalysisSchema extends ProcessSchema {
    protected function getTable(): schema\Table
    {
        return new schema\Table(name: 'biobank_specimen_analysis');
    }
}
