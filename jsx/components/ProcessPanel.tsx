import { ReactElement } from 'react';
import Form from 'Form';
const { FormElement } = Form;
import { ProcessForm } from '../components';
import { Protocol, Options } from '../types';
import { ProcessType, useSpecimenContext } from '../entities';
import { useBarcodePageContext, useBiobankContext } from '../hooks';
declare const loris: any;

// Define the type for your props
type ProcessPanelProps = {
  process: ProcessType;
  clearAll: () => void,
};

function ProcessPanel({
  process,
  clearAll,
}: ProcessPanelProps): ReactElement {

  const { edit, editable } = useBarcodePageContext();
  const { options } = useBiobankContext();
  const specimen = useSpecimenContext();

  const addProcess = async () => {
    specimen[process].set('center', specimen.container.center);
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
      return protocol.type == specimen.type &&
      protocol.process == process.replace(/^\w/, (c) => c.toUpperCase());
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
        type={editable[process] ? specimen.type : specimen.type}
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
