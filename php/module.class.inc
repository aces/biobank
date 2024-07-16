<?php declare(strict_types=1);
/**
 * This serves as a hint to LORIS that this module is a real module.
 * It does nothing but implement the module class in the module's namespace.
 *
 * PHP Version 7.2
 *
 * @category   Module
 * @package    Main
 * @subpackage Biobank
 * @author     Dave MacFarlane <david.macfarlane2@mcgill.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris-Trunk/
 */
namespace LORIS\biobank;

use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;
use \LORIS\biobank\endpoints;

/**
 * Class module implements the basic LORIS module functionality
 *
 * @category   Module
 * @package    Main
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcgill.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.txt GPLv3
 * @link       https://www.github.com/aces/Loris-Trunk/
 */
class Module extends \Module
{
    /**
     * {@inheritDoc}
     *
     * @param \User $user The user whose access is being checked.
     *
     * @return bool whether access is granted
     */
    public function hasAccess(\User $user) : bool
    {
        return parent::hasAccess($user) &&
            $user->hasAnyPermission(
                [
                    'biobank_specimen_view',
                    'biobank_container_view',
                    'biobank_pool_view',
                ]
            );
    }

    /**
     * The default handler for a module routes to a page of the first component of
     * the URL's name, or the same name as the module if serving the root of the
     * module.
     *
     * Modules extend PrefixRouter, so that the parent handler knows that the
     * css, static, and js routes should be routed through the file system to
     * retrieve the files before trying to load any specific pages.
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(ServerRequestInterface $request) : ResponseInterface
    {
        // Get the stripped path from the request URI
        $user = $request->getAttribute('user');
        $db = \NDB_Factory::singleton()->database();

        // Set the session variable for group_concat_max_len
        $db->run("SET SESSION group_concat_max_len = 536870912;");

        $path = trim($request->getURI()->getPath(), "/");
        $pathparts = explode('/', $path);

        // Determine the handler based on the stripped path
        $handler = match ($pathparts[0]) {
            'specimens' => new endpoints\Specimens($user, $db),
            'containers' => new endpoints\Containers($user, $db),
            'shipments' => new endpoints\Shipments($user, $db),
            'pools' => new endpoints\Pools($user, $db),
            default => null
        };

        // If handler is a ResponseInterface, return it directly
        if ($handler === null) {
            return parent::handle($request);
        }

        // Process the request with the determined handler
        return $handler->process($request, $handler);
    }

    /**
     * {@inheritDoc}
     *
     * @return string The menu category for this module
     */
    public function getMenuCategory() : string
    {
        return "Biobank";
    }

    /**
     * {@inheritDoc}
     *
     * @return string
     */
    public function getLongName() : string
    {
        return "Biobank";
    }

        /**
     * {@inheritDoc}
     *
     * @return bool Whether an api schema is available fro this module.
     */
    public function hasAPISpecification(): bool
    {
        return false;
    }
}
