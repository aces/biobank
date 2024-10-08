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
        $this->validateType($container);
        $this->validateTemperature($container);
        // $this->validateStatusId($container);
        // $this->validateShipmentBarcodes($container);
        $this->validateCenter($container);
        $this->validateParent($container);
        $this->validateCoordinate($container);
        // $this->_validateLotNumber($container);
        // $this->_validateExpirationDate($container);
        // $this->_validateComments($container);
    }

    private function validateBarcode(Container $container) : void
    {
        $id            = $container->id;
        $barcode       = $container->barcode;
        $prevContainer = $this->service->getByLabel($barcode);

        if (!is_null($prevContainer)) {
            // If the barcodes are identical but the ids are not
            if ($prevContainer->barcode === $barcode && $preContainer->id !== $id) {
                $this->addError('barcode', 'Barcode must be unique.');
            }

            if ($id && ($barcode !== $prevContainer->barcode)) {
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

    private function validateType(Container $container) : void
    {
    }

    private function validateTemperature(Container $container) : void
    {
        $temperature = $container->temperature;
        if (!(is_numeric($temperature) && is_float(floatval($temperature)))) {
            $this->addError('temperature', 'Temperature must be a number.');
        }
    }

    // private function validateStatus(Container $container) : void
    // {
    //     if (is_null($container->getStatus())) {
    //         $this->addError('statusId', 'Status value must be set.');
    //     }
    //     if ($container->getId()) {
    //         $prevContainer = $this->service->getInstanceFromId($container->getId());
    //         if ($prevContainer) {
    //             $stati         = $this->service->getStati();
    //             $statusLabel   = $stati[$prevContainer->getStatusId() ?? '']['label'];
    //             if ($statusLabel == 'Dispensed') {
    //                 // TODO: figure out what else should not be changed if the container
    //                 // is dispensed. This should eventually be blocked in the front-end
    //                 // as well. These validations can wait until more testing has
    //                 // been done on the module.

    //                 if ($prevContainer->getTemperature()!= $container->getTemperature()
    //                 ) {
    //                     $this->addError('temperature',
    //                         "Temperature cannot be changed if Container
    //                                            is Dispensed."
    //                     );
    //                 }
    //             }
    //         }
    //     }

    //     if (is_null($container->getStatusId())) {
    //         $this->addError('status', "Status value must be set.");
    //     }
    // }

    // private function validateShipmentBarcodes(Container $container) : void
    // {
    //     if (empty($container->shipments) {
    //         return;
    //     }

    //     $shipmentDAO = new ShipmentDAO($this->db);
    //     foreach ($container->shipments as $barcode) {
    //         error_log($barcode);
    //         $shipment = $shipmentDAO->getInstanceFromBarcode($barcode);
    //         if ($shipment->getActive()) {
    //             $this->addError('shipments',
    //                 "Container ".$container->getBarcode()." cannot
    //       be manipulated while it is associated with an active shipment."
    //             );
    //         }
    //     }

    //     // Additional shipment barcodes validation logic...
    // }

    private function validateCenter(Container $container) : void
    {
        if (false) {
            $parent = $this->service->getByLabel($container->parent);
            if ($container->center !== $parent->center) {
                $this->addError('centers',
                    $container->barcode." must be at the same location
                        as ".$parentContainer->barcode." to execute this
                        action"
                );
            }
        }
    }

    private function validateParent(Container $container) : void
    {
        $parent = $container->parent;
        if (isset($parent)) {
            $childContainerIds = $this->service->getAllChildContainerIds($container);
            if ($container->id == $parentContainerId
                || in_array($container->id, $childContainerIds)
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
        $coordinate = $container->coordinate;
        if (!is_null($coordinate)) {
            if (!((is_int($coordinate) || ctype_digit($coordinate))
                && (int)$coordinate > 0)
            ) {
                $this->addError('coordinate', 'Coordinate must be a positive integer.');
            }

            $parentContainerId = $container->parent->id;
            if (!isset($parentContainerId)) {
                $this->addError('coordinate',
                    'Coordinate can not be set without a Parent
                                       Container.'
                );
            }

            $parentContainer = $this->service->getById(
                $parentContainerId
            );

            $dimensions = $this->service->getDimensions()[$parentContainer
                ->getDimensionId() ?? ''];

            $capacity = $dimensions['x'] * $dimensions['y'] * $dimensions['z'];
            if ($container->coordinate > $capacity) {
                $this->addError('coordinate',
                    'Coordinate value exceeds the capacity of the Parent Container'
                );
            }
        }
    }
}
