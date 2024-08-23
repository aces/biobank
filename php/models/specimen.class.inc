<?php declare(strict_types=1);
/**
 * Specimen Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Specimen Object instances.
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
use LORIS\biobank\models\specimen\Type;
use LORIS\biobank\models\specimen\process\Collection;
use LORIS\biobank\models\specimen\process\Preparation;
use LORIS\biobank\models\specimen\process\Analysis;

/**
 * Specimen Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Specimen Object instances.
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
final class Specimen extends Model implements \LORIS\StudyEntities\SiteHaver
{
    public function __construct(
        public readonly ?int $id,
        public readonly ?float $quantity, //removed because parent empty? should not instantiate if empty
        public readonly ?int $fTCycle,
        public readonly ?Unit $unit,
        public readonly ?Type $type,
        public readonly ?Projects $projects,
        public readonly ?Specimens $parents,
        public readonly ?Candidate $candidate,
        public readonly ?Session $session,
        public readonly ?Pool $pool,
        public readonly ?Collection $collection,
        public readonly ?Preparation $preparation,
        public readonly ?Analysis $analysis,
        public readonly ?Container $container, //removed for the moment to account for parent specimsn not being able to loading containers but should be re-instated later.
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->container->center->id;
    }

    public function getProjectIDs(): array
    {
        return $this->projects->getIds();
    }
}
