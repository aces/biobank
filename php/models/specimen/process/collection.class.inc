<?php declare(strict_types=1);
/**
 * Collection Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Collection Object instances.
 *
 * PHP Version 7.2
 *
 * @category   Entity
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
namespace LORIS\biobank\models\specimen\process;

use LORIS\biobank\models\Unit;
use LORIS\biobank\models\specimen\Process;

/**
 * Collection Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Collection Object instances.
 *
 * PHP Version 7.2
 *
 * @category   Entity
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris/
 */
final class Collection extends Process
{
    public function __construct(
        public float $quantity,
        public Unit $unit,
        public Process $process,
    ) {
        parent::__construct($process);
        $this->quantity = $quantity,
        $this->unit = $unit,
    }
}

