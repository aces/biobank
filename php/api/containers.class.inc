<?php declare(strict_types=1);
/**
 * This implements the Container endpoint class for the Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   API
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
namespace LORIS\biobank;
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Server\RequestHandlerInterface;
use \Psr\Http\Message\ResponseInterface;

/**
 * A class for handling the /biobank/containers/ endpoint.
 *
 * PHP Version 7.2
 *
 * @category   API
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Loris/
 */
class Containers extends \NDB_Page implements RequestHandlerInterface
{
    /**
     * Returns true if user has access to this endpoint.
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
            'OPTIONS',
        ];
    }

    /**
     * This function can be overridden in a module's page to load the necessary
     * resources to check the permissions of a user.
     *
     * @param User                   $user    The user to load the resources for
     * @param ServerRequestInterface $request The PSR15 Request being handled
     *
     * @return void
     */
    public function loadResources(
        \User $user, ServerRequestInterface $request
    ) : void {
    }

    /**
     * This function passes the request to the handler. This is necessary since
     * the Endpoint bypass the Module class.
     *
     * XXX: This function should be extracted to a parent class.
     *
     * @param ServerRequestInterface  $request The PSR7 request.
     * @param RequestHandlerInterface $handler The request handler.
     *
     * @return ResponseInterface The outgoing PSR7 response.
     */
    public function process(
        ServerRequestInterface $request,
        RequestHandlerInterface $handler
    ) : ResponseInterface {
        return $handler->handle($request);
    }



// XXX: Idea for routing library
// // Define a route to get all containers
// $router->get('/containers', function () use ($contCont) {
//     $filteredContainers = $contCont->getInstances();
//     return new \LORIS\Http\Response\JSON\OK($filteredContainers);
// });
// 
// // Define a route to get a specific container by barcode
// $router->get('/containers/{barcode}', function ($barcode) use ($contCont) {
//     $container = $contCont->getInstanceByBarcode($barcode);
//     return new \LORIS\Http\Response\JSON\OK($container);
// });

    /**
     * Handles Endpoint requests.
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(ServerRequestInterface $request) : ResponseInterface
    {
        ini_set('memory_limit', '-1');
        $db       = \NDB_Factory::singleton()->database();
        $user     = $request->getAttribute('user');

        $uri      = $request->getUri()->getPath();
        $segments = explode('/', trim($uri, '/'));
        $secondLastSegment = count($segments) > 1 ? $segments[count($segments) - 2] : null;


        // TODO: this needs to be workshopped...
        $limit = null;
        $queryParams = $request->getQueryParams();
        if (isset($queryParams['limit'])) {
            $limit = (int)$queryParams['limit'];
            unset($queryParams['limit']);
        }
        
        $offset = 0;
        if (isset($queryParams['offset'])) {
            $offset = (int)$queryParams['offset'];
            unset($queryParams['offset']);
        }

        $contCont = new ContainerController($this->loris, $user);

        try {
            $db->beginTransaction();
            switch ($request->getMethod()) {
            case 'GET':
                $lastSegment = end($segments);
                if ($secondLastSegment === 'containers' && !empty($lastSegment)) {
                    $barcode  = end($segments);
                    $container = $contCont->getInstanceByBarcode($barcode);
                    return new \LORIS\Http\Response\JSON\OK($container);
                } else {
                    // Use query parameters for filtering
                    $filteredContainers = $contCont->getInstances($queryParams, $limit, $offset);
                    return new \LORIS\Http\Response\JSON\OK($filteredContainers);
                }
            case 'POST':
                $containers        = json_decode(
                    $request->getBody()->getContents(),
                    true
                );
                $createdContainers = $contCont->createInstances($containers);
                return new \LORIS\Http\Response\JSON\OK($createdContainers);
            case 'PUT':
              $lastSegment = end($segments);
              if ($secondLastSegment === 'containers' && !empty($lastSegment)) {
                  $barcode = $lastSegment;  // Assuming this is the container ID
                  $container = json_decode($request->getBody()->getContents(), true);
                  
                  // Validate container and container here

                  $updatedContainer = $contCont->updateInstance($container);
                  return new \LORIS\Http\Response\JSON\OK(['data' => $updatedContainer]);
              } else {
                  // Handle invalid URL or missing container ID
                  return new \LORIS\Http\Response\JSON\BadRequest("Invalid request");
              }
                $container         = json_decode(
                    $request->getBody()->getContents(),
                    true
                );
                $updatedContainers = $contCont->updateInstance($container);
                return new \LORIS\Http\Response\JSON\OK($updatedContainers);
            case 'OPTIONS':
                return (new \LORIS\Http\Response())
                    ->withHeader('Allow', $this->allowedMethods());
            }
        } catch (BadRequest $e) {
            $db->rollBack();
            return new \LORIS\Http\Response\JSON\BadRequest($e->getMessage());
        } catch (Forbidden $e) {
            $db->rollBack();
            return new \LORIS\Http\Response\JSON\Forbidden($e->getMessage());
        } catch (\Exception $e) {                                               
            $db->rollBack();                                                    
            // PUT SOMETHING HERE LATER                                         
            return new \LORIS\Http\Response\JSON\InternalServerError(           
                $e->getMessage()                                                
            ); 
        } finally {
            if ($db->inTransaction()) {
                $db->commit();
            }
        }
    }
}
