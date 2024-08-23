<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\Data\Provisioners\ModelProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\data\DAO;
use LORIS\biobank\data\schemas\CandidateSchema;
use LORIS\biobank\data\schema\LoadingStrategy;
use LORIS\biobank\models\Candidate;

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
            new ModelProvisioner(new CandidateSchema('candidate'))
        );
    }

    /**
     * Handles actions to be performed after saving a Candidate.
     *
     * @param Model $candidate The saved Candidate instance.
     */
    public function handlePostSave(Model $Candidate): void {}
}
