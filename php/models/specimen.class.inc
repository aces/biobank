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

use LORIS\biobank\models\Container;
use LORIS\biobank\models\specimen;
use LORIS\biobank\models\Center;
use LORIS\biobank\models\Projects;
use LORIS\biobank\models\Specimens;
use LORIS\biobank\models\Candidate;
use LORIS\biobank\models\Session;
use LORIS\biobank\models\Pool;
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
class Specimen extends Model implements \LORIS\StudyEntities\SiteHaver
{
    public function __construct(
        private int $id
        public Container $container,
        public specimen\Type $type,
        public float $quantity,
        public string $unit,
        public Projects $projects,
        public int $fTCycle,
        public Specimens $parents,
        public Candidate $candidate,
        public Session $session,
        public Pool $pool,
        public Collection $collection,
        public Preparation $preparation,
        public Analysis $analysis
    ) {}

    public getCenterID(): \CenterID
    {
        return $this->container->center->id;
    }

    public getProjectIDs(): array
    {
        return array_map(function($project) {
            return $project->getId();
        }, $this->projects);
    }
}
