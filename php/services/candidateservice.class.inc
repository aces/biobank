<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\Model;
use LORIS\biobank\Service;
use LORIS\biobank\provisioners\CandidateProvisioner;

/**
 * Service class for handling Candidate operations.
 *
 * This class initializes the necessary components for Candidate operations
 * and provides specific implementations for handling post-save actions.
 */
class CandidateService extends Service
{
    function __construct()
    {
        parent::__construct(
            new CandidateProvisioner(),
            new LORIS\biobank\Factory(LORIS\biobank\models\Candidate::class)
        );
    }

    /**
     * Handles actions to be performed after saving a Candidate.
     *
     * @param Model $candidate The saved Candidate instance.
     */
    public function handlePostSave(Model $Candidate): void {}
}
