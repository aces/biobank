<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use \LORIS\biobank\Service;
use \LORIS\biobank\Model;
use \LORIS\biobank\provisioners\ShipmentProvisioner;
use \LORIS\biobank\validators\ShipmentValidator;

/**
 * Service class for handling Container operations.
 *
 * This class initializes the necessary components for Container operations
 * and provides specific implementations for handling post-save actions.
 */
class ShipmentService extends Service
{
    function __construct()                                                         
    {                                                                              
        parent::__construct(                                                       
            new ShipmentProvisioner(),
            new \LORIS\biobank\Factory(\LORIS\biobank\models\Shipment::class),
            new ShipmentValidator($this),                                         
        );                                                                         
    }          

    /**
     * Handles actions to be performed after saving a Container.
     *
     * @param Model $instance The saved Container instance.
     */
    public function handlePostSave(Model $shipment): void
    {
        $this->saveLogs($shipment);
        $this->saveContainers($shipment);
    }

    public function saveLogs(Shipment $shipment): void
    {
        
        // $logDAO = new DAO($dao->db);
        // foreach ($shipment->getLogs() as $log) {
        //     $log->setShipmentId($shipment->getId());
        //     $logDAO->save($log);
        // }
    }

    public function saveContainers(Shipment $shipment): void
    {
        //$containerDAO = new ContainerDAO($dao->db);
        //foreach ($shipment->getContainerIds() as $containerId) {
        //    $container = $containerDAO->getInstanceById((int) $containerId);
        //    if (!$shipment->getId()) {
        //        $container->setParentContainerId(null);
        //    } else {
        //        $container->setCenterId(new \CenterID(strval($shipment->getDestinationCenterId())));
        //    }
        //    $containerDAO->save($container);
        //}
    }
}
