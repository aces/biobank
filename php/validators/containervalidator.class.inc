<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Validator;
use LORIS\biobank\Model;
use LORIS\biobank\models\Container;

class ContainerValidator extends Validator
{

    protected function getUniqueFields(): array {
        return ['barcode'];
    }

    protected function getRequiredFields() : array {
        return ['barcode', 'type', 'temperature', 'status'];
    }

    protected function getRequiredIfFields() : array
    {
        return [];
    }

    protected function getFieldTypes() : array {
        return [
            'barcode' => 'string',
            'type' => 'string',
            'temperature' => 'float',
            'status' => 'string',
        ];
    }

    protected function validateFields(Model $container) : void
    {
        $this->validateBarcode($container);
        $this->validateTypeId($container);
        $this->validateTemperature($container);
        $this->validateStatusId($container);
        $this->validateShipmentBarcodes($container);
        $this->validateCenterId($container);
        $this->validateParentContainerId($container);
        $this->validateCoordinate($container);
        // $this->_validateLotNumber($container);
        // $this->_validateExpirationDate($container);
        // $this->_validateComments($container);
    }

    private function validateBarcode(Container $container) : void
    {
        $id            = $container->getId();
        $barcode       = $container->getBarcode();
        $containerList = $this->dao->selectInstances(
            [
                ['column'=>'bc.Barcode', 'value'=>$barcode],
            ],
        );

        if (!empty($containerList)) {
            foreach ($containerList as $c) {
                $b = $c->getBarcode();
                $i = $c->getId();
                // If the barcodes are identical but the ids are not
                if ($b === $barcode && $i !== $id) {
                    $this->addError('barcode', 'Barcode must be unique.');
                }
            }
        }

        if ($id) {
            $prevContainer = $this->dao->getInstanceFromId($id);
            if ($prevContainer && ($container->getBarcode() !== $prevContainer->getBarcode())) {
                $this->addError('barcode', 'Barcode can not be changed.');
            }
        }

        // TODO: Regex check on Barcode. Currently, regex has been removed from
        // the schema. Once it's reintroduced, this can be implemented.
        //$specimenTypes = $this->dao->getSpecimenTypes();
        //$regex = $specimenTypes[$specimen['typeId']]['regex'];
        //if (isset($regex)) {
        //    if (preg_match($regex, $container['barcode']) !== 1) {
        //        throw new BadRequest('Barcode is not of proper format for the
        //                               selected specimen type');
        //    }
        //}
    }

    private function validateTypeId(Container $container) : void
    {
        if (is_null($container->getTypeId())) {
            $result->addError('typeId', 'Type value must be set.');
        }
    }

    private function validateTemperature(Container $container) : void
    {
        $temperature = $container->getTemperature();
        if (is_null($temperature)) {
            $result->addError('temperature', 'Temperature value must be set.');
        }
        if (!(is_numeric($temperature) && is_float(floatval($temperature)))) {
            $result->addError('temperature', 'Temperature must be a number.');
        }
    }

    private function validateStatusId(Container $container) : void
    {
        if (is_null($container->getStatusId())) {
            $result->addError('statusId', 'Status value must be set.');
        }
        if ($container->getId()) {
            $prevContainer = $this->dao->getInstanceFromId($container->getId());
            if ($prevContainer) {
                $stati         = $this->dao->getStati();
                $statusLabel   = $stati[$prevContainer->getStatusId() ?? '']['label'];
                if ($statusLabel == 'Dispensed') {
                    // TODO: figure out what else should not be changed if the container
                    // is dispensed. This should eventually be blocked in the front-end
                    // as well. These validations can wait until more testing has
                    // been done on the module.

                    if ($prevContainer->getTemperature()!= $container->getTemperature()
                    ) {
                        $this->addError('temperature',
                            "Temperature cannot be changed if Container
                                               is Dispensed."
                        );
                    }
                }
            }
        }

        if (is_null($container->getStatusId())) {
            $this->addError('status', "Status value must be set.");
        }
    }

    private function validateShipmentBarcodes(Container $container) : void
    {
        if (empty($container->getShipmentBarcodes())) {
            return;
        }

        $shipmentDAO = new ShipmentDAO($this->db);
        foreach ($container->getShipmentBarcodes() as $barcode) {
            error_log($barcode);
            $shipment = $shipmentDAO->getInstanceFromBarcode($barcode);
            if ($shipment->getActive()) {
                $this->addError('shipments',
                    "Container ".$container->getBarcode()." cannot
          be manipulated while it is associated with an active shipment."
                );
            }
        }

        // Additional shipment barcodes validation logic...
    }

    private function validateCenterId(Container $container) : void
    {
        if (is_null($container->getCenterID())) {
            $result->addError('centerId', 'Center value must be set.');
        }

        $parentContainerId = $container->getParentContainerId();
        if (false) {
            $parentContainer = $this->dao->getInstanceFromId($parentContainerId);
            if ($container->getCenterID() !== $parentContainer->getCenterID()) {
                $this->addError('centers',
                    $container->getBarcode()." must be at the same location
                        as ".$parentContainer->getBarcode()." to execute this
                        action"
                );
            }
        }
    }

    private function validateParentContainerId(Container $container) : void
    {
        $parentContainerId = $container->getParentContainerId();
        if (isset($parentContainerId)) {
            $childContainerIds = $this->dao->getAllChildContainerIds($container);
            if ($container->getId() == $parentContainerId
                || in_array($container->getId(), $childContainerIds)
            ) {
                $this->addError('parentContainers',
                    'A container can not be placed within itself or
                    within one of its descendant containers.'
                );
            }
        }
    }

    private function validateCoordinate(Container $container) : void
    {
        $coordinate = $container->getCoordinate();
        if (!is_null($coordinate)) {
            if (!((is_int($coordinate) || ctype_digit($coordinate))
                && (int)$coordinate > 0)
            ) {
                $this->addError('coordinate', 'Coordinate must be a positive integer.');
            }

            $parentContainerId = $container->getParentContainerId();
            if (!isset($parentContainerId)) {
                $this->addError('coordinate',
                    'Coordinate can not be set without a Parent
                                       Container.'
                );
            }

            $parentContainer = $this->dao->getInstanceFromId(
                $parentContainerId
            );

            $dimensions = $this->dao->getDimensions()[$parentContainer
                ->getDimensionId() ?? ''];

            $capacity = $dimensions['x'] * $dimensions['y'] * $dimensions['z'];
            if ($container->getCoordinate() > $capacity) {
                $this->addError('coordinate',
                    'Coordinate value exceeds the capacity of the Parent Container'
                );
            }
        }
    }
}
