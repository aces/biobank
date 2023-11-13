import React from 'react';
import PropTypes from 'prop-types';
import SpecimenProcessForm from './processForm';
import {clone} from './helpers.js';


// TODO: Replace 'any' and 'Function' with appropriate declarations
type SpecimenProps = {
  current = any,
  editable = any,
  errors = any,
  options = any,
  specimen = any,
  container = any,
  editSpecimen = Function,
  edit = Function,
  clearAll = Function,
  editable = boolean,
  process = 'collection' | 'preparation' | 'analysis',
  setCurrent = Function,
  setSpecimen = Function,
  updateSpecimen = Function,
}

/**                                                                             
 * Specimen Component for displaying specimen information and actions.            
 *                                                                              
 * @component                                                                   
 * @param {SpecimenProps} props - The properties passed to the component.         
 * @returns {ReactElement} React element representing the Specimen.               
 */  
const BiobankSpecimen = ({
  current,
  editable,
  errors,
  options,
  specimen,
  container,
  editSpecimen,
  edit,
  clearAll,
  editable,
  process,
  setCurrent,
  setSpecimen,
  updateSpecimen,
  }: SpecimenProps) => {
  const addProcess = async (process) => {
    const newSpecimen = clone(specimen);
    newSpecimen[process] = {centerId: container.centerId};
    await editSpecimen(newSpecimen);
    edit(process);
  };

  const alterProcess = (process) => {
    editSpecimen(specimen)
    .then(() => edit(process));
  };

  return (
    <div className="processing">
      <Processes
        addProcess={addProcess}
        alterProcess={alterProcess}
        specimen={specimen}
        editable={editable}
        clearAll={clearAll}
        current={current}
        errors={errors}
        options={options}
        setCurrent={setCurrent}
        setSpecimen={setSpecimen}
        updateSpecimen={updateSpecimen}
      >
        <ProcessPanel process='collection'/>
        <ProcessPanel process='preparation'/>
        <ProcessPanel process='analysis'/>
      </Processes>
    </div>
  );
}

/**
 * React component to display processes
 *
 * @component
 * @param {object} props - React props
 * @returns {ReactElement} React element
 */
function Processes(props) {
  return React.Children.map(props.children, (child) => {
    return React.cloneElement(child, {...props});
  });
}

// TODO: Replace 'any' and 'Function' with appropriate declarations
type ProcessPanelProps = {
  editable = any,
  process = 'collection' | 'preparation' | 'analysis',
  current = any,
  specimen = any,
  options = any,
  alterProcess = Function,
  clearAll = Function,
  addProcess,
  errors = any,
  setCurrent = Function,
  setSpecimen = Function,
  updateSpecimen = Function,
}

/**
 * React component to display a panel of processes
 *
 * @component
 * @param {ProcessPanelProps}
 * @return {ReactElement}
 */
const ProcessPanel({
  editable,
  process,
  current,
  specimen,
  options,
  alterProcess,
  clearAll,
  addProcess,
  errors,
  setCurrent,
  setSpecimen,
  updateSpecimen,
  }: ProcessPanelProps) {

  const alterProcess = () => {
    if (loris.userHasPermission('biobank_specimen_alter')) {
      return (
        <span
          className={editable[process] ? null : 'glyphicon glyphicon-pencil'}
          onClick={editable[process] ? null : () => alterProcess(process)}
        />
      );
    }
  };

  const cancelAlterProcess = () => {
    if (editable[process]) {
      return (
        <a
          className="pull-right"
          style={{cursor: 'pointer'}}
          onClick={clearAll}
        >
          Cancel
        </a>
      );
    }
  };

  const protocolExists = Object.values(options.specimen.protocols).find(
    (protocol) => {
      return protocol.typeId == specimen.typeId &&
      options.specimen.processes[protocol.processId].label ==
      process.replace(/^\w/, (c) => c.toUpperCase());
    }
  );

  let panel = null;
  if (protocolExists &&
      !specimen[process] &&
      !editable[process] &&
      loris.userHasPermission('biobank_specimen_update')) {
    panel = (
      <div className='panel specimen-panel inactive'>
        <div className='add-process' onClick={() => addProcess(process)}>
          <span className='glyphicon glyphicon-plus'/>
        </div>
        <div>ADD {process.toUpperCase()}</div>
      </div>
    );
  }

  const form = (
    <FormElement>
      <SpecimenProcessForm
        current={current}
        errors={errors.specimen[process]}
        edit={editable[process]}
        specimen={current.specimen}
        options={options}
        process={
          editable[process] ?
          current.specimen[process] :
          specimen[process]
        }
        processStage={process}
        setCurrent={setCurrent}
        setParent={setSpecimen}
        typeId={editable[process] ? current.specimen.typeId : specimen.typeId}
        updateSpecimen={updateSpecimen}
      />
    </FormElement>
  );

  if (specimen[process] || editable[process]) {
    panel = (
      <div className='panel specimen-panel panel-default'>
        <div className='panel-heading'>
          <div className={'lifecycle-node '+process}>
            <div className='letter'>
              {process.charAt(0).toUpperCase()}
            </div>
          </div>
          <div className='title'>
            {process.replace(/^\w/, (c) => c.toUpperCase())}
          </div>
          {alterProcess()}
        </div>
        <div className='panel-body'>
          {form}
          {cancelAlterProcess()}
        </div>
      </div>
    );
  }

  return panel;
}

export default BiobankSpecimen;
