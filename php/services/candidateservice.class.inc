<?php declare(strict_types=1);

namespace LORIS\biobank\services;

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
    function __construct(\Database $db)
    {
        $dao = new DAO($db,
            new CandidateSchema(
                name: 'candidate',
                strategy: LoadingStrategy::EAGER
            ), new Factory(Candidate::class));
        parent::__construct($dao);
    }

    /**
     * Handles actions to be performed after saving a Candidate.
     *
     * @param Model $candidate The saved Candidate instance.
     */
    public function handlePostSave(Model $Candidate): void {}
}
