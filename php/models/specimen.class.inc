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
        public readonly float $quantity,
        public readonly ?int $fTCycle = null,
        private readonly ?int $id = null,
        public readonly ?Unit $unit = null,
        public readonly ?Type $type = null,
        public readonly ?Projects $projects = null,
        public readonly ?Specimens $parents = null,
        public readonly ?Candidate $candidate = null,
        public readonly ?Session $session = null,
        public readonly ?Pool $pool = null,
        public readonly ?Collection $collection = null,
        public readonly ?Preparation $preparation = null,
        public readonly ?Analysis $analysis = null,
        public readonly ?Container $container = null, //removed for the moment to account for parent specimsn not being able to loading containers but should be re-instated later.
    ) {}

    public function getCenterID(): \CenterID
    {
        return $this->container->center->id;
    }

    public function getProjectIDs(): array
    {
        return array_map(function($project) {
            return $project->getId();
        }, $this->projects);
    }
}
