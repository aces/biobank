<?php declare(strict_types=1);
/**
 * Pool Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Pool Object instances.
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
namespace LORIS\biobank\Models;

use LORIS\biobank\models\Unit;
use LORIS\biobank\models\Specimens;
use LORIS\biobank\models\Canditate;
use LORIS\biobank\models\Session;
use LORIS\biobank\models\container\Type;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\Projects;
use LORIS\biobank\types\Date;
use LORIS\biobank\types\Time;

/**
 * Pool Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Pool Object instances.
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
class Pool extends Model implements \LORIS\StudyEntities\SiteHaver
{
     public function __construct(
        private int $id,
        public string $label,
        public float $quantity,
        public Unit $unit,
        public Specimens $specimens,
        public Date $date,
        public Time $time
    ) {}

    public getCenterID(): \CenterID
    {
        return $this->specimens[0]->center->id;
    }

    public getProjectIDs(): array
    {
        return $this->specimens[0]->getProjectIDs();
    }
}

