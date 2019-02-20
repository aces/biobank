<?php declare(strict_types=1);
/**
 * This implement the Specimen endpoint class for the Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   API
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <hrabalais.mcin@gmail.com>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
namespace LORIS\biobank;
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;

/**
 * A class for handling the /biobank/specimenendpoint/ endpoint.
 *
 * PHP Version 7.2
 *
 * @category   Biobank
 * @package    Main
 * @subpackage Biobank
 * @author     Henri Rabalais <hrabalais.mcin@gmail.com>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Lors/
 */
class SpecimenEndpoint extends \LORIS\Api\Endpoint
{
    /**
     * Returns true if user has acces to this Controller.
     *
     * @param \User $user The user whose access is being checked
     *
     * @return bool
     */
    function _hasAccess(\User $user) : bool
    {
        return true;
    }

    /**
     * Return which methods are supported by this endpoint.
     *
     * @return array supported HTTP methods
     */
    protected function allowedMethods() : array
    {
        return [
                'GET',
                'PUT',
                'POST',
               ];
    }

    /**
     * Versions of LORIS API which are supported by this endpoint.
     *
     * @return array supported HTTP methods.
     */
    protected function supportedVersions() : array
    {
        return ['v0.0.3-dev'];
    }

    /**
     * Handles Controller requests
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(ServerRequestInterface $request) : ResponseInterface
    {
        $db       = \Database::singleton();
        $user     = $request->getAttribute('user');
        $specCont = new SpecimenController($db, $user);

        try {
            switch($request->getMethod()) {
            case 'GET':
                $specimens = $specCont->getInstances();
                return \LORIS\Http\Response\OK($specimens);
            case 'POST':
                $specimens = json_decode($request->getBody()->getContents(), true);
                $files     = $request->getUploadedFiles()['files'];
                $specCont->createInstances($specimens, $files);
                return \LORIS\Http\Response\OK();
            case 'PUT':
                $specimens = json_decode($request->getBody()->getContents(), true);
                $files     = $request->getParsedBody()['files'];
                $specCont->updateInstances($specimens, $files);
                return \LORIS\Http\Response\OK();
            }
            case 'OPTIONS':
                return (new \LORIS\Http\Response())
                    ->withHeader('Allow', $this->allowedMethods());
        } catch (\Invalid $e) {
            return \LORIS\Http\Response\BadRequest($e->getMessage());
        } catch (\Forbidden $e) {
            return \LORIS\Http\Response\Forbidden($e->getMessage());
        }
    }
}
