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

use LORIS\biobank\provisioners\PoolProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\validators\PoolValidator;
use LORIS\biobank\models\Pool;
use LORIS\Data\Filters; 

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
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            (new PoolProvisioner())
                ->filter(new Filters\UserSiteMatch())                                  
                ->filter(new Filters\UserProjectMatch()),
            new Factory(Pool::class),
            new PoolValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Container.
     *
     * @param Model $instance The saved Container instance.
     */
    protected function handlePostSave(Model $instance): void
    {
        if (!$instance instanceof Pool) {
            throw new \InvalidArgumentException('Expected instance of Pool');
        }

        // TODO: Implement post-save logic specific to Container
        // Example: Handle parent association changes
    }
}
