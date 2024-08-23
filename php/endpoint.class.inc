<?php declare(strict_types=1);

namespace LORIS\biobank;

use \Psr\Http\Server\RequestHandlerInterface;                                      
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;
use LORIS\Http\Endpoint as LORISEndpoint;
use LORIS\biobank\data\query\QueryParamsHandler;
use LORIS\biobank\endpoints\Permission;

/**
 * A class for handling the /biobank/specimens/ endpoint.
 *
 * @category Biobank
 * @package  Loris
 * @author   Henri Rabalais <henri.rabalais@mcin.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link     http://www.github.com/aces/Lors/
 */
abstract class Endpoint
    extends LORISEndpoint
    // implements \LORIS\Middleware\ETagCalculator
{
    /**
     * A cache of the results of the projects/ endpoint, so that it doesn't
     * need to be recalculated for the ETag and handler
     */
    private $_cache;
    private $controller;

    public function __construct(\Loris\LorisInstance $loris)
    {
        parent::__construct($loris);
        $this->controller = new Controller($this->getService());
    }

    // Abstract method to initialize the controller
    abstract protected static function getPermission(
        Permission $permission
    ): string;
    abstract protected function getSubHandler(string $subpath): ?Endpoint;

    /**
     * Return which methods are supported by this endpoint.
     *
     * @return array supported HTTP methods
     */
    protected function allowedMethods() : array
    {
        return ['GET', 'PUT', 'POST', 'OPTIONS'];
    }

    protected function checkPermission(
        \User $user,
        Permission $permission
    ) : void
    {                                                                              
        if (!$user->hasPermission($this->getPermission($permission))) {
            throw new Forbidden(
                get_called_class() . ": {$permission->name()} Permission Denied"
            );
        }
    }     

    public function process(
        ServerRequestInterface $request,
        RequestHandlerInterface $handler
    ): ResponseInterface
    {
        $path = trim($request->getURI()->getPath(), "/");
        $pathparts = explode('/', $path);

        // Determine subendpoint handler based on the subpath
        if (count($pathparts) > 1) {
            $subpath = $pathparts[1];
            $subhandler = $this->getSubHandler($pathparts[1]);

            if ($subhandler !== null) {
                // Remove the handled part of the path and pass the remainder
                $remainingPath = implode('/', array_slice($pathparts, 1));

                // Update the request URI path
                $uri = $request->getUri()->withPath($remainingPath);
                $request = $request->withUri($uri);

                return $subhandler->process($request, $subhandler);
            }
        }

        // Default processing for the main endpoint if no subendpoint matches
        return parent::process($request, $handler);
    }

    /**
     * Handles Endpoint requests
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *
     * @return ResponseInterface The outgoing PSR7 response
     */
    public function handle(
        ServerRequestInterface $request
    ) : ResponseInterface
    {
        return match ($request->getMethod()) {
            'GET' => $this->_handleGET($request),
            'POST' => $this->_handlePOST($request),
            'PUT' => $this->_handlePUT($request),
            'OPTIONS' => (new \LORIS\Http\Response())
                ->withHeader('Allow', $this->allowedMethods()),
            default => new \LORIS\Http\Response\JSON\MethodNotAllowed(
                $this->allowedMethods()
            ),
        };
    }

    private function _handleGET(
        ServerRequestInterface $request
    ): ResponseInterface
    {
        $this->checkPermission($request->getAttribute('user'), Permission::GET);
    
        $path = trim($request->getURI()->getPath(), "/");
        $pathparts = explode('/', $path);
    
        $queryParams = (new QueryParamsHandler(
            $request->getQueryParams()))->parse();
    
        if (count($pathparts) === 1) {
            return $this->controller->handleRootRequest(
                $request, $queryParams, $this->_cache);
        } else {
            return $this->controller->handleResourceRequest(
                $request, $pathparts[1]);
        }
    }

    private function _handlePUT(
        ServerRequestInterface $request
    ): ResponseInterface
    {
        $this->checkPermission($request->getAttribute('user'), Permission::PUT);
    
        $path = trim($request->getURI()->getPath(), "/");
        $pathparts = explode('/', $path);
    
        if (count($pathparts) === 2) {
            return $this->controller->handleUpdateRequest(
                $request, $pathparts[1]
            );
        } else {
            throw new \LORIS\Http\Exception\NotFoundException("Invalid path.");
        }
    }

    private function _handlePOST(
        ServerRequestInterface $request
    ): ResponseInterface
    {
        $user = $request->getAttribute('user');
        $this->getPermission($user, Permission::POST);
    
        $contentType = $request->getHeaderLine('Content-Type');
    
        if (strpos($contentType, 'multipart/form-data') !== false) {
            $uploadedFiles = $request->getUploadedFiles();
            return $this->controller->handleFileUploadRequest(
                $request, $uploadedFiles
            );
        } else {
            return $this->controller->handleCreateRequest($request);
        }
    }    

    /**
     * Implements the ETagCalculator interface
     *
     * @param ServerRequestInterface $request The PSR7 incoming request.
     *
     * @return string etag summarizing value of this request.
     */
    //public function ETag(ServerRequestInterface $request) : string
    //{
    //    return md5(json_encode($this->_handleGET($request)->getBody()));
    //}
}
