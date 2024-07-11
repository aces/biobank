<?php declare(strict_types=1);
/**
 * This implement the Specimen endpoint class for the Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category Biobank
 * @package  Loris
 * @author   Henri Rabalais <henri.rabalais@mcin.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link     http://www.github.com/aces/Loris/
 */
namespace LORIS\biobank;

use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;
use LORIS\Http\Endpoint as LORISEndpoint;
use LORIS\biobank\data\query\QueryParamsHandler;

/**
 * A class for handling the /biobank/specimens/ endpoint.
 *
 * @category Biobank
 * @package  Loris
 * @author   Henri Rabalais <henri.rabalais@mcin.ca>
 * @license  http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link     http://www.github.com/aces/Lors/
 */
abstract class Endpoint extends LORISEndpoint implements \LORIS\Middleware\ETagCalculator
{
    /**
     * A cache of the results of the projects/ endpoint, so that it doesn't
     * need to be recalculated for the ETag and handler
     */
    private $_cache;
    private $controller;
    private $user;

    public function __construct(\User $user, Controller $controller)
    {
        $this->user = $user;
        $this->controller = $controller;
    }

    // Abstract method to initialize the controller
    abstract protected static function getPermission(Permission $permission): string;
    abstract protected static function getName(): string;

    /**
     * Return which methods are supported by this endpoint.
     *
     * @return array supported HTTP methods
     */
    protected function allowedMethods() : array
    {
        return ['GET', 'PUT', 'POST', 'OPTIONS'];
    }

    protected function checkPermission(Permission $permission) : bool                       
    {                                                                              
        if (!$this->user->hasPermission($this->getPermission($permission))) {
            throw new Forbidden($this->getName() . ": {$permission->name()} Permission Denied");
        }
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
        // ini_set('memory_limit', '-1');

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
        $this->getPermission(Permission::GET);

        // TODO: not convinced this is the best way to unpack the path. It feels
        // like should already be done by the module.
        $path = trim($request->getURI()->getPath(), "/");
        $pathparts = explode('/', $path);
        $queryParams = (new QueryParamsHandler($request->getQueryParams()))->parse();

        if (count($pathparts) === 1) {
            if (isset($this->_cache)) {
                return $this->_cache;
            }

            $result = $this->controller->get($queryParams);

            $this->_cache = new \LORIS\Http\Response\OK($result);

            return $this->_cache;
        } else {
            $id  = $pathparts[1];                                 
            $result = $this->controller->getById($id, $queryParams);     
            return new \LORIS\Http\Response\JSON\OK($result);        
        }

        // XXX: might be needed to properly send data....
        // $specimens = (new \LORIS\Data\Table())
        //     ->withDataFrom($this->controller->dao)
        //     ->toArray($user);
    }

    private function _handlePOST(
        ServerRequestInterface $request
    ): ResponseInterface
    {                                                                           
        $this->checkPermission(Permission::POST);

        $data = json_decode((string) $request->getBody()->getContents(), true);                
        $result = $this->controller->create($data);

        if (!empty($request->getUploadedFiles())) {
            $response = $this->uploadFiles($request);
            if ($response->getStatusCode() !== 200) {
                return $response;
            }
        }

        return new \LORIS\Http\Response\JSON\OK($result);
    }       

    private function _handlePUT(                                                
        ServerRequestInterface $request                                         
    ) : ResponseInterface
    {                                                     
        $this->checkPermission(Permission::PUT);

        $data = json_decode($request->getBody()->getContents(), true);
        if (json_last_error() !== JSON_ERROR_NONE) {
            return new \LORIS\Http\Response\JSON\BadRequest(
                ['error' => 'Invalid JSON provided']
            );
        }

        $result = $this->controller->update($data);
        return new \LORIS\Http\Response\JSON\OK($result);
    }  

    /**
     * Validates and Uploades files
     *
     * @param ServerRequestInterface $request The incoming PSR7 request
     *                                        containg the files to be uploaded.
     *
     * @return ResponseInterface
     */
    public function uploadFiles(ServerRequestInterface $request) : ResponseInterface
    {
        $config = \NDB_Config::singleton();
        $path   = $config->getSetting('biobankPath');

        if (!isset($path)) {
            throw new \NotFound('Biobank File Upload Path is not configured');
        }
        $targetdir = new \SplFileInfo($path);

        $uploader = (new \LORIS\FilesUploadHandler($targetdir))
            ->withPermittedMIMETypes(
                'text/plain'
            );

        return $uploader->handle($request);
    }

    /**
     * Implements the ETagCalculator interface
     *
     * @param ServerRequestInterface $request The PSR7 incoming request.
     *
     * @return string etag summarizing value of this request.
     */
    public function ETag(ServerRequestInterface $request) : string
    {
        return md5(json_encode($this->_handleGET($request)->getBody()));
    }
}
