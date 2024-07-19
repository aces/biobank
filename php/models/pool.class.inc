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
namespace LORIS\biobank\models;

use LORIS\biobank\Model;
use LORIS\biobank\models\container\Type;
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
final class Pool extends Model implements \LORIS\StudyEntities\SiteHaver
{
     public function __construct(
        public readonly string $label,
        public readonly float $quantity,
        public readonly Date $date,
        public readonly Time $time,
        private readonly ?int $id = null,
        public readonly ?Unit $unit = null,
        public readonly ?Specimens $specimens = null,
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->specimens[0]->center->id;
    }

    public function getProjectIDs(): array
    {
        return $this->specimens[0]->getProjectIDs();
    }
}
