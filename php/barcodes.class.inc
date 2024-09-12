<?php

namespace LORIS\biobank;

use \LORIS\Http\Endpoint;
use Psr\Http\Message\ServerRequestInterface;
use Psr\Http\Message\ResponseInterface;

class Barcodes extends Endpoint
{
    protected $db;
    protected $user;

    /**                                                                         
     * Returns true if user has access to this Endpoint.                        
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
        return ['GET', 'OPTIONS'];                                          
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
     * Handles Endpoint requests                                                
     *                                                                          
     * @param ServerRequestInterface $request The incoming PSR7 request         
     *                                                                          
     * @return ResponseInterface The outgoing PSR7 response                     
     */                                                                         
    public function handle(
        ServerRequestInterface $request
    ) : ResponseInterface {
        $this->db = \NDB_Factory::singleton()->database();
        $this->user = $request->getAttribute('user');

        try {
            switch ($request->getMethod()) {
                case 'GET':
                    return $this->_handleGET($request);
                case 'OPTIONS':
                    return (new \LORIS\Http\Response())
                        ->withHeader('Allow', $this->allowedMethods());
            }
        } catch (\Exception $e) {
            $this->db->rollBack();
            return new \LORIS\Http\Response\JSON\InternalServerError(
                $e->getMessage()
            );
        }
    }

    // NOTE: the algorithm can stay here but should 
    /**                                                                         
     * Handle the logic of an incoming HTTP GET request                         
     *                                                                          
     * @param ServerRequestInterface $request The incoming PSR7 request
     * @return ResponseInterface                                                 
     */                                                                         
    private function _handleGET(
        ServerRequestInterface $request
    ) : ResponseInterface {
        $currentYear = date('y');

        $queryParams = $request->getQueryParams();
        $count = isset($queryParams['count'])
            ? intval($queryParams['count'])
            : 1;

        // Query to get the largest barcode for the current year
        $query = "SELECT MAX(CAST(SUBSTRING(Barcode, 3, 6) AS UNSIGNED)) AS max_number
                  FROM biobank_container
                  WHERE SUBSTRING(Barcode, 1, 2) = :year";
        $params = [':year' => $currentYear];
        $maxNumber = $this->db->pselectOne($query, $params);

        $newNumber = $maxNumber
            ? $maxNumber + 1
            : 1;

        $barcodes = [];
        for ($i = 0; $i < $count; $i++) {
            $newNumberFormatted = str_pad(
                $newNumber + $i,
                6,
                '0',
                STR_PAD_LEFT
            );
            $newBarcode = $currentYear . $newNumberFormatted;
            $barcodes[] = $newBarcode;
        }

        return new \LORIS\Http\Response\JSON\OK(['barcodes' => $barcodes]);
    }
}
