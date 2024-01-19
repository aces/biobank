import { ReactElement } from 'react;
import { FormElement } from 'Form'; // Replace with actual import path
import { ProcessForm } from '../components'; // Replace with actual import path
import { Specimen, SpecimenHandler, Options, ProcessType } from '../types';
import { useBarcodePageContext } from '../hooks';
declare const loris: any;

// Define the type for your props
type ProcessPanelProps = {
  process: ProcessType;
  specimen: Specimen; // Define a more specific type if possible
  specHandler: SpecimenHandler,
  clearAll: () => void,
  options: Options, // Define a more specific type if possible
};

function ProcessPanel({
  process,
  specimen,
  specHandler,
  clearAll,
  options,
}: ProcessPanelProps): ReactElement {

  const { edit, editable } = useBarcodePageContext();

  const addProcess = async () => {
    specHandler.setProcess(process, 'centerId', specimen.container.centerId);
    edit(process);
  };

  const alterProcess = () => {
    if (loris.userHasPermission('biobank_specimen_alter')) {
      return (
        <span
          className={editable[process] ? null : 'glyphicon glyphicon-pencil'}
          onClick={editable[process] ? null : () => edit(process)}
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
        <div className='add-process' onClick={addProcess}>
          <span className='glyphicon glyphicon-plus'/>
        </div>
        <div>ADD {process.toUpperCase()}</div>
      </div>
    );
  }

  const form = (
    <FormElement>
      <ProcessForm
        edit={editable[process]}
        specimen={specimen}
        options={options}
        process={specimen[process]}
        processStage={process}
        setParent={specHandler.set}
        typeId={editable[process] ? specimen.typeId : specimen.typeId}
        updateSpecimen={specHandler.put}
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

export default ProcessPanel;
