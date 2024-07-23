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

use LORIS\biobank\types\Time;
use LORIS\biobank\types\Date;
use LORIS\biobank\models\Unit;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\Examiner;
use LORIS\biobank\models\specimen\Protocol;
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
        ?int $id = null,
        ?Center $center = null,
        ?Examiner $examiner = null,
        ?Date $date = null,
        ?Time $time = null,
        ?string $comments = null,
        ?array $data = null,
        ?Protocol $protocol = null
    ) {
        parent::__construct($id, $center, $examiner, $date, $time, $comments, $data, $protocol);
    }
}
