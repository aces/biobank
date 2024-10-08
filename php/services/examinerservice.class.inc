<?php declare(strict_types=1);

namespace LORIS\biobank\services;

use LORIS\biobank\provisioners\ExaminerProvisioner;
use LORIS\biobank\Service;
use LORIS\biobank\Model;
use LORIS\biobank\Factory;
use LORIS\biobank\models\Examiner;

/**
 * Service class for handling Examiner operations.
 *
 * This class initializes the necessary components for Examiner operations
 * and provides specific implementations for handling post-save actions.
 */
class ExaminerService extends Service
{
    function __construct()
    {
        parent::__construct(
            new ExaminerProvisioner(),
            new LORIS\biobank\Factory(LORIS\biobank\models\Examiner::class)
        );
    }

    /**
     * Handles actions to be performed after saving a Examiner.
     *
     * @param Model $examiner The saved Examiner instance.
     */
    public function handlePostSave(Model $examiner): void {}
}
