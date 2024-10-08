<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\provisioners\DiagnosisProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\Diagnosis;

/**
 * Service class for handling Diagnosis operations.
 *
 * This class initializes the necessary components for Diagnosis operations
 * and provides specific implementations for handling post-save actions.
 */
class DiagnosisService extends Service
{
    function __construct()
    {
        parent::__construct(
            new DiagnosisProvisioner(),
            new Factory(Diagnosis::class)
        );
    }

    /**
     * Handles actions to be performed after saving a Diagnosis.
     *
     * @param Model $diagnosis The saved Diagnosis instance.
     */
    public function handlePostSave(Model $Diagnosis): void {}
}
