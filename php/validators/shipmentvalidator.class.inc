<?php declare(strict_types=1); 

namespace LORIS\biobank\validators;

use LORIS\biobank\Model;
use LORIS\biobank\Validator;
use LORIS\biobank\models\Shipment;

class ShipmentValidator extends Validator
{

    protected function getUniqueFields(): array {
        return ['barcode'];
    }

    protected function getRequiredFields() : array {
        return ['barcode', 'type', 'destinationCenter', 'containers', 'logs'];
    }

    protected function getFieldTypes() : array {
        return [
            'barcode' => 'string',
            'type' => 'string',
            'destinationCenter' => 'string',
            'containers' => 'array',
            'logs' => 'array'
        ];
    }

    protected function getRequiredIfFields(): array
    {
        return [];
    }

    protected function validateFields(Model $shipment) : void
    {
        $this->validateBarcode($shipment);
        $this->validateLogs($shipment);
        $this->validateContainers($shipment);
    }

    private function validateBarcode($shipment): void {
        if (!$shipment->id) {
            foreach ($this->dao->selectInstances() as $shipmentComp) {
                if ($shipment->barcode === $shipmentComp->barcode) {
                    $this->addError('barcode', 'A shipment with this Barcode already exists.');
                }
            }
        }
    }

    private function validateLogs($shipment): void {
        $logValidator = new LogValidator($this->dao);
        foreach ($shipment->logs as $log) {
            $logValidator->validate($log);
            if ($logValidator->hasErrors()) {
                $this->errors = array_merge($this->errors, $logValidator->getErrors());
            }
        }
    }

    private function validateContainers(Shipment $shipment) : array
    {
        foreach ($shipment->containerIds as $containerId) {
            $container = $containerDAO->getInstanceFromId(
                (int) $containerId
            );

            // TODO: Re-Implement once integrity of Centers of Containers is
            // established in CBIGR. Currently, child containers are permitted
            // at a different site than the parent. This should NOT be the case.

            /* Check that containers are all at the same center.*/
            // if ($container->getCenterID() != $container1->getCenterID()) {
            //     $errors[$field] = $container->getBarcode().' and '
            //     .$container1->getBarcode().' are not at the same Center. All
            //     Containers must be at the same Center.';
            //     return $errors;
            // }

            // Check that containers are not already associated with an active
            // shipment.
            if (!$shipment->id) {
                foreach ($container->getShipmentBarcodes() as $shipmentBarcode) {
                    $containerShipment = $this->dao->getInstanceFromBarcode(
                        $shipmentBarcode
                    );
                    if ($containerShipment->getActive()) {
                        $errors[$field] = $container->getBarcode().' is already
                  associated with an active shipment';
                    }
                };
            }
        }

        // TODO: this could maybe be improved since it only checks 1 shipment.
        // TODO: This check needs to be reworked because it will only apply for
        // the shipment creation.
        // Check that the containers are at the same center as the shipment.
        // if ($shipment->logs[0]->centerId !== $container1->getCenterID()) {
        //     $error .= 'Containers must be at the same site as the shipment. ';
        // }

    }
}
