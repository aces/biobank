<?php declare(strict_types=1);

namespace LORIS\biobank\Endpoints;

use LORIS\biobank\Endpoint;

class Labels extends Endpoint
{
    public function __construct(\User $user, \Database $db)
    {
        $dao = (new SpecimenDAO($db))->filter(new UserProjectMatch());
        $service = new LabelService($dao);
        parent::__construct($user, $service);
    }

    protected function _hasAccess(string $permission) : bool
    {
        return true;
    }

    protected function _handlePOST(ServerRequestInterface $request): ResponseInterface
    {
        $params = json_decode($request->getBody()->getContents(), true);
        if (json_last_error() !== JSON_ERROR_NONE) {
            return new \LORIS\Http\Response\JSON\BadRequest(['error' => 'Invalid JSON provided']);
        }

        try {
            foreach ($params as $param) {
                $this->printLabel($param);
            }
            return new \LORIS\Http\Response\JSON\OK(['message' => 'Labels printed successfully']);
        } catch (\Exception $e) {
            return new \LORIS\Http\Response\JSON\InternalServerError(['error' => $e->getMessage()]);
        }
    }

    private function printLabel(array $param): void
    {
        $barcode   = $param['barcode'];
        $type      = $param['type'];
        $pscid     = $param['pscid'];
        $sampleNum = $param['sampleNumber'];

        // Construct ZPL string
        $zpl = "^XA"
             . "^FO330,80"
             . "^BY1"
             . "^BCN,100,Y,Y,N"
             . "^CFA,20"
             . "^A0N,30,29"
             . "^BCN,100,Y,Y,N^FD$barcode^FS"
             . "^FO323,190"
             . "^CFA,20"
             . "^A0N,30,29^FD$type^FS"
             . "^FO323,250"
             . "^CFA,20"
             . "^A0N,30,29^FD$pscid $sampleNum^FS"
             . "^XZ";

        $config  = \NDB_Config::singleton();
        $path    = $config->getSetting('labelPrintingPath') . 'barcode.zpl';

        // Ensure directory exists
        $directory = dirname($path);
        if (!is_dir($directory)) {
            mkdir($directory, 0777, true);
        }

        // Write ZPL to file
        if (false === ($fp = fopen($path, 'w'))) {
            throw new \RuntimeException("Unable to open file: $path");
        }
        if (false === fwrite($fp, $zpl)) {
            fclose($fp);
            throw new \RuntimeException("Unable to write to file: $path");
        }
        fclose($fp);

        // Send the file to the printer
        $output = shell_exec('lp -d ZDesigner -o raw ' . escapeshellarg($path));
        if ($output === null) {
            throw new \RuntimeException("Failed to send print job to printer");
        }
    }
}
