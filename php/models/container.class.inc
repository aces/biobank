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
use LORIS\biobank\models\Container;
use LORIS\biobank\models\Shipment;
use LORIS\biobank\models\Center;
use LORIS\biobank\type\Date;

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
        private int $id,
        public string $barcode,
        public Container\Type $type,
        public float $temperature,
        public ?Shipments $shipments,
        public Center $center,
        public ?Container $parent,
        public ?Containers $children,
        public ?int $coordinate,
        public ?string $lotNumber,
        public Date $expirationDate,
        public ?string $comments,
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->center->id;
    }
}
