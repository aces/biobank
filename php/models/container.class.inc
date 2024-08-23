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
        private ?int $id,
        public ?string $barcode,
        public ?float $temperature,
        public ?Type $type,
        public ?Center $center,
        public ?Shipments $shipments, //Shipments[]
        public ?Container $parent,
        public ?Containers $children, //Containers[]
        public ?int $coordinate,
        public ?string $lotNumber,
        public ?Date $expirationDate,
        public ?string $comments,
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->center->id;
    }
}
