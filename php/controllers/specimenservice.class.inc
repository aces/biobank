<?php declare(strict_types=1);
/**
 * This class features the code that enables access to the Specimens of the LORIS
 * Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   Service
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Lors/
 */
namespace LORIS\biobank\services;

/**
 * This class features the code that enables access to the Specimens of the LORIS
 * Biobank Module.
 *
 * PHP Version 7.2
 *
 * @category   Service
 * @package    Loris
 * @subpackage Biobank
 * @author     Henri Rabalais <henri.rabalais@mcin.ca>
 * @license    http://www.gnu.org/licenses/gpl-3.0.text GPLv3
 * @link       http://www.github.com/aces/Lors/
 */
class SpecimenService
{
    private SpecimenDAO $dao;

    function __construct(SpecimenDAO $dao)
    {
        $this->dao = $dao;
    }

    public function getInstances() : array
    {
        return $this->dao->getAllInstances();
    }

    public function getInstanceById(string $id) : array                      
    {                                                                           
        $specimen = $this->dao->find($id);
        if (!$specimen) {
            throw new Exception("Specimen not found");
        }
        return $specimen;
    }

    public function createInstances(array $list) : array
    {
        $this->dao->db->beginTransaction();
        try {
            $specimens = [];
            foreach (array_values($list) as $i => $specimen) {
                $specimen = (new Specimen())->fromArray($specimenArray);
                $this->_validateInstance($specimen);
                $specimens[]  = $this->dao->saveInstance($specimen);
            }
            $this->dao->db->commit();
            return $specimens;
        } catch (Exception $e) {
            $this->dao->db->rollback();
            throw $e;
        }
    }

    public function updateInstance(array $specimenArray) : Specimen
    {
        $this->dao->db->beginTransaction();
        try {
            $specimen = (new Specimen())->fromArray($specimenArray);
            $this->_validateInstance($specimen);
            $updatedSpecimen = $this->dao->saveInstance($specimen);
            $this->dao->db->commit();
            return $updateSpecimen;
        } catch (Exception $e) {
            $this->dao->db->rollback();
            throw $e;
        }
    }

    public function getFields() : array
    {
        return [
            'types'              => $this->dao->getTypes(),
            'processes'          => $this->dao->getProcesses(),
            'units'              => $this->dao->getUnits(),
            'attributes'         => $this->dao->getAttributes(),
        ];
    }

    public function getConfig() : array
    {
        return [
            'type' => $this->dao->getTypeConfig(),
            'protocols'          => $this->dao->getProtocols(),
            'protocolAttributes' => $this->dao->getProtocolAttributes(),
            'processAttributes'  => $this->dao->getProcessAttributes(),
        ];

    }

    private function _validateInstance(Specimen $specimen) : void
    {
        $this->_validateTypeId($specimen);
        $this->_validateQuantity($specimen);
        $this->_validateUnitId($specimen);
        $this->_validateFTCycle($specimen);
        $this->_validateParentSpecimenIds($specimen);
        $this->_validateCandidateId($specimen);
        $this->_validateSessionId($specimen);
        $this->_validateCollection($specimen);
        $this->_validatePreparation($specimen);
        $this->_validateAnalysis($specimen);
    }

    /**
     * Validates Specimen Object Type ID.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateTypeId(Specimen $specimen) : void
    {
        if (is_null($specimen->getTypeId())) {
            throw new BadRequest("Specimen Type value must be set");
        }
    }

    /**
     * Validates Specimen Object Quantity.
     *
     * @param Specimen|Process $object Object to be validated.
     *
     * @throws BadRequest if the provided object does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateQuantity(object $object) : void
    {
        $quantity = $object->getQuantity();
        if (is_null($quantity)) {
            throw new BadRequest('Specimen Quantity value must be set');
        }
        if (!(is_numeric($quantity) && is_float(floatval($quantity)))) {
            throw new BadRequest('Specimen Quantity value must be a number');
        }
        //TODO: This is where a capacity check would go, i.e. $quantity > $capacity.
        if ($quantity < 0) {
            throw new BadRequest('Specimen Quantity cannot be less than 0');
        }
    }

    /**
     * Validates Specimen Object Unit ID.
     *
     * @param Specimen|Process $object Object to be validated.
     *
     * @throws BadRequest if the provided object does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateUnitId(object $object) : void
    {
        if (is_null($object->getUnitId())) {
            throw new BadRequest("Specimen Unit value must be set");
        }
    }

    /**
     * Validates Specimen Object FT Cycle.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateFTCycle(Specimen $specimen) : void
    {
        $fTCycle = $specimen->getFTCycle();
        if (isset($fTCycle)) {
            if (strval($fTCycle) !== strval(intval($fTCycle))
                || intval($fTCycle) < 0
            ) {
                throw new BadRequest("F/T Cycle must be 0 or more");
            }

            if ($fTCycle > 100) {
                throw new BadRequest("F/T Cycle value is too large");
            }
        }
    }

    /**
     * Validates Specimen Object Parent Specimen IDs.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
                          requirements.
     *
     * @return void
     */
    private function _validateParentSpecimenIds(Specimen $specimen) : void
    {
        $parentSpecimenIds = $specimen->getParentSpecimenIds();
        if ($parentSpecimenIds) {
            $baseParentSpecimen = $this->dao->getInstanceFromId(
                (int) $parentSpecimenIds[0]
            );
        }
        $specimenTypes = $this->dao->getTypes();

        if (empty($parentSpecimeIds)) {
            return;
        }
        if (empty($baseParentSpecimen)) {
            return;
        }
        foreach ($parentSpecimenIds as $parentSpecimenId) {
            $parentSpecimen = $this->dao->getInstanceFromId((int) $parentSpecimenId);

            $pSpecimenCandId = $parentSpecimen->getCandidateId();

            if ($baseParentSpecimen->getCandidateId() != $pSpecimenCandId) {
                throw new BadRequest(
                    'Parent Specimens must be from the same Candidate'
                );
            }

            if ($baseParentSpecimen->getSessionId()!= $parentSpecimen->getSessionId()
            ) {
                throw new BadRequest(
                    'Parent Specimens must be from the same Session'
                );
            }

            if ($baseParentSpecimen->getTypeId() != $parentSpecimen->getTypeId()) {
                throw new BadRequest('Parent Specimens must be of the same Type');
            }

            if ($parentSpecimen->getTypeId() != $specimen->getTypeId()) {
                if (empty($specimenTypes[$specimen->getTypeId()]['parentTypeIds'])) {
                    $parentTypeId = $parentSpecimen->getTypeId();

                    $childType  = $specimenTypes[$specimen->getTypeId()]['label'];
                    $parentType = $specimenTypes[$parentTypeId]['label'];
                    throw new BadRequest(
                        "A specimen of type '$childType' cannot be derived
                                           from a parent of type '$parentType'"
                    );
                }
                if (!in_array(
                    $parentSpecimen->getTypeId(),
                    $specimenTypes[$specimen->getTypeId()]['parentTypeIds']
                )
                ) {
                    throw new BadRequest(
                        'Parent Specimens must be of a type that
                                           is a parent to the current Specimen type'
                    );
                }
            }
        }
    }

    /**
     * Validates Specimen Object Candidate ID.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateCandidateId(Specimen $specimen) : void
    {
        if (is_null($specimen->getCandidateId())) {
            throw new BadRequest("PSCID value must be set");
        }
    }

    /**
     * Validates Specimen Object Session ID.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateSessionId(Specimen $specimen) : void
    {
        if (is_null($specimen->getSessionId())) {
            throw new BadRequest("Visit Label value must be set");
        }
    }

    /**
     * Validates Specimen Object Collection.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateCollection(Specimen $specimen) : void
    {
        $collection = $specimen->getCollection();
        $this->_validateProtocolId($collection);
        $this->_validateExaminerId($collection);
        $this->_validateQuantity($collection);
        $this->_validateUnitId($collection);
        $this->_validateCenterId($collection);
        // $this->_validateDate($collection);
        // $this->_validateTime($collection);
        $this->_validateData($collection);
    }

    /**
     * Validates Specimen Object Preparation.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validatePreparation(Specimen $specimen) : void
    {
        $preparation = $specimen->getPreparation();
        if (isset($preparation)) {
            $this->_validateProtocolId($preparation);
            $this->_validateExaminerId($preparation);
            $this->_validateCenterId($preparation);
            // $this->_validateDate($preparation);
            // $this->_validateTime($preparation);
            $this->_validateData($preparation);
        }
    }

    /**
     * Validates Specimen Object Type ID.
     *
     * @param Specimen $specimen Specimen to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateAnalysis(Specimen $specimen) : void
    {
        // Validate Analysis
        $analysis = $specimen->getAnalysis();
        if (isset($analysis)) {
            $this->_validateProtocolId($analysis);
            $this->_validateExaminerId($analysis);
            $this->_validateCenterId($analysis);
            // $this->_validateDate($analysis);
            // $this->_validateTime($analysis);
            $this->_validateData($analysis);
        }
    }

    /**
     * Validates Specimen Process Protocol ID.
     *
     * @param Process $process Process to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateProtocolId(Process $process) : void
    {
        if (is_null($process->getProtocolId())) {
            throw new BadRequest('Specimen Processing Protocol must be set');
        }
    }

    /**
     * Validates Specimen Process Examiner ID.
     *
     * @param Process $process Process to be validated.
     *
     * @throws BadRequest if the provided Specimen does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateExaminerId(Process $process) : void
    {
        if (is_null($process->getProtocolId())) {
            throw new BadRequest('Specimen Processing Examiner must be set');
        }
        // TODO: Potentially check that examinerId is within the set of possible
        // values. I.e. an ID of the examiner table.
    }

    /**
     * Validates Specimen Process Center ID.
     *
     * @param Process $process Process to be validated.
     *
     * @throws BadRequest if the provided Process does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateCenterId(Process $process) : void
    {
        if (is_null($process->getCenterId())) {
            throw new BadRequest('Specimen Processing Center must be set');
        }
    }

    /**
     * Validates Specimen Process Data.
     *
     * @param Process $process Process to be validated.
     *
     * @throws BadRequest if the provided Process does not meet validation
     *                    requirements.
     *
     * @return void
     */
    private function _validateData(Process $process) : void
    {
        $protocolAttributes = $this->dao->getProtocolAttributes();
        $attributeDatatypes = $this->dao->getAttributeDatatypes();
        $protocolId         = $process->getProtocolId();
        $attributes         = $protocolAttributes[$protocolId] ?? [];
        $data = $process->getData() ?? [];

        foreach ($attributes as $id => $attribute) {
            if ($attribute['required'] == true && !isset($data[$id])) {
                throw new BadRequest(
                    "The attribute "
                    . $attribute['label']
                    . " is required. Protocol "
                    . $protocolId
                );
            }
        }

        foreach ($data as $attributeId => $value) {
            $attribute = $attributes[$attributeId];

            if (!isset($attribute)) {
                $wrongAttribute = $this->dao->getAttributes()[$attributeId];
                throw new BadRequest(
                    "The attribute '"
                    . $wrongAttribute['label']
                    . "' does not belong to this protocol"
                );
            }

            $datatype = $attributeDatatypes[$attribute['datatypeId']]['datatype'];
            if ($datatype === 'boolean' && !is_bool($value)) {
                throw new BadRequest(
                    "The attribute '"
                    . $attribute['label']
                    . "' must be a boolean"
                );
            }

            if ($datatype === 'number' && !is_numeric($value)) {
                throw new BadRequest(
                    "The attribute '"
                    . $attribute['label']
                    . "' must be a number"
                );
            }

            // TODO: This validation may not be necessary, but it may be good to
            // leave this if statement here just as a reminder that it exists.
            if ($datatype === 'text') {
            }

            // TODO: This can likely be validated through the DateTime Object.
            if ($datatype === 'date') {
            }

            // TODO: This can likely be validated through the DateTime Object.
            if ($datatype === 'time') {
            }

            // TODO: Set this validate when files start to get uploaded.
            if ($datatype === 'file') {
            }
        }
    }
}
