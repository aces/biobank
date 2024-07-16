<?php declare(strict_types=1);
/**
 * Container Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Container Object instances.
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

/**
 * Container Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Container Object instances.
 *
 * PHP Version 7.2
 *
 * @category Loris
 * @package  Biobank
 * @author   Henri Rabalais <hrabalais.mcin@gmail.com>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://www.github.com/aces/Loris/
 */

class Container extends Model implements \LORIS\StudyEntities\SiteHaver
{
    public function __construct(
        private ?int $id = null,
        public ?string $barcode = null,
        public ?float $temperature = null,
        public ?Type $type = null,
        public ?Center $center = null,
        public ?Shipments $shipments = null, //Shipments[]
        public ?Container $parent = null,
        public ?Containers $children = null, //Containers[]
        public ?int $coordinate = null,
        public ?string $lotNumber = null,
        public ?Date $expirationDate = null,
        public ?string $comments = null,
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->center->id;
    }
}
