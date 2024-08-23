import { ReactElement } from 'react';
import { ProcessForm } from '../forms';
import { IProtocol, ProcessType, useSpecimenContext } from '../entities';
import { useRequest, useBarcodePageContext, useBiobankContext } from '../hooks';
import { Button } from '../forms'
import { SpecimenAPI } from '../APIs';

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

  // TODO: replace with API that does this itself....
  const protocolExists = Object.values(useRequest(new SpecimenAPI().getProtocols()))
    .find(
    (protocol: IProtocol) => {
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

  const updateButton = specimen && (
    <Button
      label="Update"
      onClick={ () => new SpecimenAPI().update(specimen)}
    />
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
          <ProcessForm
            edit={editable[process]}
            process={specimen[process]}
            processStage={process}
            type={editable[process] ? specimen.type : specimen.type}
          />
          {cancelAlterProcess()}
          {updateButton}
        </div>
      </div>
    );
  }

  return panel;
}

export default ProcessPanel;
