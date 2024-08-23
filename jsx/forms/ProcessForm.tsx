import { ReactElement } from 'react';
import { Button, CustomFields, ProcessField } from '../forms';
import { ISpecimenType, ProcessHook, ProcessProvider} from '../entities';

type ProcessFormProps = {
  process: ProcessHook,
  processStage: string,
  type: ISpecimenType,
  edit?: boolean,
  hideProtocol?: boolean,
};

function ProcessForm({
  process,
  processStage,
  type,
  edit,
  hideProtocol,
}: ProcessFormProps): ReactElement {

  if (!type) return null;

  return (
    <ProcessProvider process={process}>
      {!hideProtocol && (
        <ProcessField property={'protocol'}/>
      )}
      <ProcessField property={'examiner'}/>
      <ProcessField property={'date'}/>
      <ProcessField property={'time'}/>
      {processStage === 'collection' && (
        <>
          <ProcessField property={'quantity'}/>
          <ProcessField property={'unit'}/>
        </>
      )}
      <div className='form-top'/>
      <ProcessField property={'comments'}/>
    </ProcessProvider>
  );
};

export default ProcessForm;
