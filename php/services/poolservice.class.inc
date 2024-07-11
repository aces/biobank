<?php declare(strict_types=1);
/**
 * This class features the code that enables access to the Pools of the
 * LORIS Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   Service
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
namespace LORIS\biobank\services;

use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\PoolSchema;
use LORIS\biobank\validators\PoolValidator;
use LORIS\biobank\models\Pool;
use LORIS\Data\Filters\UserProjectMatch; 

/**
 * This class features the code that enables access to the Pools of the
 * LORIS Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   Service
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
class PoolService extends Service
{
    public function __construct()
    {
        $dao = (new DAO(\NDB_Factory::singleton()->database(), PoolSchema::eager()))->filter(new UserProjectMatch()); 
        $validator = new PoolValidator($dao);

        parent::__construct($dao, $validator);
    }

    /**
     * Handles actions to be performed after saving a Container.
     *
     * @param Model $instance The saved Container instance.
     */
    protected function handlePostSave(Model $instance): void
    {
        if (!$instance instanceof Pool) {
            throw new \InvalidArgumentException('Expected instance of Container');
        }

        // TODO: Implement post-save logic specific to Container
        // Example: Handle parent association changes
    }
}
