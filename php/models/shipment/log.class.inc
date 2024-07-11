<?php declare(strict_types=1);
/**
 * Log Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Log Object instances.
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
namespace LORIS\biobank\models;

use LORIS\biobank\models\Shipment;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\shipment\log\Status;
use LORIS\biobank\models\User;
use LORIS\biobank\type\Date;
use LORIS\biobank\type\Time;

/**
 * Log Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Log Object instances.
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
class Log extends Model
{
    public function __construct(
        protected Shipment $shipment,
        protected Center $center,
        protected Status $status,
        protected User $user,
        protected float $temperature,
        protected Date $date,
        protected Time $time,
        protected string $comments
    ) {}
}
