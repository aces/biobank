<?php declare(strict_types=1);

namespace LORIS\biobank;

use LORIS\Http\DataIteratorJSONStream;
use LORIS\biobank\data\query\QueryParams;
use \Psr\Http\Message\ServerRequestInterface;
use \Psr\Http\Message\ResponseInterface;

class Controller
{
    public function __construct(
        protected Service $service
    ) {}

    public function handleRootRequest(
        ServerRequestInterface $request,
        QueryParams $queryParams,
        ?ResponseInterface &$cache
    ): ResponseInterface {
        // if (isset($cache)) {
        //     error_log('CACHE IS SET');
        //     return $cache;
        // }
        $user = $request->getAttribute('user');
        $models = (new \LORIS\Data\Table())
            ->withDataFrom($this->service->get($queryParams));
        if (($request->getQueryParams()['format'] ?? '') === 'json') {
            $cache = (new \LORIS\Http\Response())
                ->withHeader("Content-Type", "application/octet-stream")
                ->withBody(new DataIteratorJSONStream($models->getRows($user)));
        } else {
             $cache = new \LORIS\Http\Response\JSON\OK($models->toArray($user));
        }

        return $cache;
    }

    public function handleResourceRequest(
        ServerRequestInterface $request,
        string $resourceId
    ): ResponseInterface {
        $result = $this->service->getInstanceByLabel($resourceId);
        return new \LORIS\Http\Response\JSON\OK([$result]);
    }

    public function handleUpdateRequest(
        ServerRequestInterface $request,
        string $resourceId
    ): ResponseInterface {
        $data = json_decode($request->getBody()->getContents(), true);
        $result = $this->service->update($resourceId, $data);
        return new \LORIS\Http\Response\JSON\OK($result);
    }    

    public function handleCreateRequest(
        ServerRequestInterface $request
    ): ResponseInterface {
        $data = json_decode($request->getBody()->getContents(), true);
        $result = $this->service->create($data);
        return new \LORIS\Http\Response\JSON\Created($result);
    }    

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
