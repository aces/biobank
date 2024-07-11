<?php declare(strict_types=1);
/**
 * Shipment Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Shipment Object instances.
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

use LORIS\biobank\models\Center;
use LORIS\biobank\models\Containers;
use LORIS\biobank\models\shipment\Logs;
use LORIS\biobank\models\shipment\Type;

/**
 * Shipment Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Shipment Object instances.
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
class Shipment extends Model implements \LORIS\StudyEntities\MultiSiteHaver
{
    public function __construct(
        private int $id,
        public string $barcode,
        public Type $type,
        public Center $destinationCenter,
        public Containers $containers,
        public Logs $logs
    ) {}

    public function getOriginCenter(): ?Center
    {
        return !empty($this->logs) ? $this->logs[0]->center : null;
    }

    public function getStatus(): ?string
    {
        return !empty($this->logs) ? $this->logs[array_key_last($this->logs)]->status : null;
    }

    public function isActive(): bool
    {
        return $this->getStatus() !== 'received';
    }

    public function getCenterIds(): array
    {
        switch ($this->getStatus()) {
       // TODO: Instead of checking for strings, this ideally would check for
       // boolean values from functions such as getActive(). This way, the
       // strings will be relegated to singular functions that then serve
       // functional purposes.
            case 'received':
                return [$this->destinationCenter->id];
            case 'shipped':
                return [$this->getOriginCenter()->id, $this->destinationCenter->id];
            default:
                return [$this->getOriginCenter()->id];
        }
    }

    public function toJSON(): array 
    {
        return [
            'originCenter' => getOriginCenter(),
            'status' => getStatus(),
            'active' => isActive(),
        ];
    }
}
