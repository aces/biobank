<?php declare(strict_types=1);
/**
 * Preparation Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Preparation Object instances.
 *
 * PHP Version 7.2
 *
 * @category Entity
 * @package  Biobank
 * @author   Henri Rabalais <henri.rabalais@mcin.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link     https://www.github.com/aces/Loris/
 */

namespace LORIS\biobank;

/**
 * Preparation Object Class
 * This class contains an instance of a database handling that is needed to
 * permanently store and retrieve Preparation Object instances.
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
class Preparation extends Process
{
    /**
     * Instatiates a Preparation Object using the values sent from the client-side.
     *
     * @param array $data Data to be used to instantiate the Preparation Object.
     *
     * @return Process
     */
    public function fromArray(array $data) : Process
    {
        parent::fromArray($data);

        return $this;
    }

    /**
     * A wrapper function for the jsonSerialize method of the Preparation Instance
     * Class. This converts this Specimen Preparation into an array
     *
     * @return array Specimen Array
     */
    public function toArray()
    {
        return $this->jsonSerialize();
    }

    /**
     * Returns an array representing the state of the Specimen.
     *
     * @return array
     */
    public function jsonSerialize() : array
    {
        $parent = parent::jsonSerialize();
        return array_merge(
            $parent,
            []
        );
    }
}

