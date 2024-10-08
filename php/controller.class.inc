<?php declare(strict_types=1);

namespace LORIS\biobank;

use LORIS\Http\DataIteratorJSONStream;
use LORIS\biobank\data\query\QueryParams;
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;
use LORIS\Http\Response\JSON as Response;


/**
 * Controller class responsible for handling HTTP requests related to Biobank
 * services.
 *
 * This class provides methods for handling various HTTP requests such as
 * retrieving resources, updating resources, and uploading files. It interacts
 * with a Service class to manage the data.
 *
 * @package    LORIS
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.j.rabalais@gmail.com>
 * @license    https://www.gnu.org/licenses/gpl-3.0.html GNU General Public
 *             License
 */
class Controller
{
    /**
     * Constructs the Controller with the provided service.
     *
     * @param Service $service The service that handles business logic and data
     *                         manipulation.
     */    
    public function __construct(
        protected Service $service
    ) {}

    /**
     * Handles the root request for retrieving models.
     *
     * This method handles requests to retrieve a list of models, applying the
     * specified query parameters and returning the result in JSON format if
     * requested.
     *
     * @param ServerRequestInterface $request    The HTTP request.
     * @param QueryParams            $queryParams The parameters for querying
     *                                            the data.
     * @param ?ResponseInterface     &$cache      Optional reference to a cache
     *                                            object.
     * @return ResponseInterface                  The response, either in JSON
     *                                            format or a default format.
     */    
    public function handleRootRequest(
        ServerRequestInterface $request,
        QueryParams $queryParams,
        ?ResponseInterface &$cache
    ): ResponseInterface {
        if (isset($cache)) {
            error_log('CACHE IS SET');
            return $cache;
        }
        $user = $request->getAttribute('user');
        $models = (new \LORIS\Data\Table())
            ->withDataFrom($this->service->apply($queryParams));
        if (($request->getQueryParams()['format'] ?? '') === 'json') {
            $cache = (new \LORIS\Http\Response())
                ->withHeader("Content-Type", "application/octet-stream")
                ->withBody(new DataIteratorJSONStream($models->getRows($user)));
        } else {
             $cache = new Response\OK($models->toArray($user));
        }

        return $cache;
    }

    /**
     * Handles a request to retrieve a specific resource by its ID.
     *
     * @param ServerRequestInterface $request    The HTTP request.
     * @param string                 $resourceId The ID of the resource to
     *                                            retrieve.
     * @return ResponseInterface                  The response containing the
     *                                            requested resource data.
     */    
    public function handleResourceRequest(
        ServerRequestInterface $request,
        string $resourceId
    ): ResponseInterface {
        $result = $this->service->getInstanceByLabel($resourceId);
        return new Response\OK([$result]);
    }

    /**
     * Handles a request to update a resource.
     *
     * @param ServerRequestInterface $request    The HTTP request containing the
     *                                            update data.
     * @param string                 $resourceId The ID of the resource to update.
     * @return ResponseInterface                  The response containing the
     *         
     */
    public function handleUpdateRequest(
        ServerRequestInterface $request,
        string $resourceId
    ): ResponseInterface {
        $data = json_decode($request->getBody()->getContents(), true);
        $result = $this->service->update($resourceId, $data);
        return new Response\OK($result);
    }    

    /**
     * Handles a request to create a new resource.
     *
     * @param ServerRequestInterface $request The HTTP request containing the
     *                                        data to create the resource.
     * @return ResponseInterface              The response containing the created
     *                                        resource data.
     */
    public function handleCreateRequest(
        ServerRequestInterface $request
    ): ResponseInterface {
        $data = json_decode($request->getBody()->getContents(), true);
        $result = $this->service->save(...$data);
        if ($result['status'] === 'error') {
            // Handle validation errors
            return new \LORIS\Http\Response\JsonResponse($result, 400);
        }
        return new Response\Created($result['data']);
    }    

    /**
     * Handles file upload requests for biobank files.
     *
     * This method handles file uploads, ensuring the file type is permitted and
     * saving the file to the configured biobank path.
     *
     * @param ServerRequestInterface $request The HTTP request containing the
     *                                        file upload data.
     * @return ResponseInterface              The response indicating the
     *                                        success or failure of the upload.
     * @throws \NotFound If the biobank file upload path is not configured.
     */    
    public function handleFileUploadRequest(
        ServerRequestInterface $request
    ): ResponseInterface {
        $config = \NDB_Config::singleton();
        $path = $config->getSetting('biobankPath');

        if (!isset($path)) {
            throw new \NotFound('Biobank File Upload Path is not configured');
        }

        $targetdir = new \SplFileInfo($path);

        $uploader = (new \LORIS\FilesUploadHandler($targetdir))
            ->withPermittedMIMETypes('text/plain');

        return $uploader->handle($request);
    }
}    
