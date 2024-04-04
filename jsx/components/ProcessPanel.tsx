import { ReactElement } from 'react';
import Form from 'Form';
const { FormElement } = Form;
import { ProcessForm } from '../components';
import { Protocol, Specimen, Options, ProcessType } from '../types';
import { useBarcodePageContext, useBiobankContext } from '../hooks';
declare const loris: any;

// Define the type for your props
type ProcessPanelProps = {
  process: ProcessType;
  specimen: Specimen; // Define a more specific type if possible
  clearAll: () => void,
};

function ProcessPanel({
  process,
  specimen,
  clearAll,
}: ProcessPanelProps): ReactElement {

  const { edit, editable } = useBarcodePageContext();
  const { options } = useBiobankContext();

  const addProcess = async () => {
    specimen.setProcess(process, 'centerId', specimen.container.centerId);
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
    (protocol: Protocol) => {
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
        process={specimen[process]}
        processStage={process}
        setParent={specimen.set}
        typeId={editable[process] ? specimen.typeId : specimen.typeId}
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
