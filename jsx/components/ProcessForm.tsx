import { ReactElement } from 'react';
import { mapFormOptions, clone } from '../utils';
import { CustomFields, ProcessField } from '../components';
import { IProtocol, ISpecimenType, ProcessHook, SpecimenHook } from '../entities';
import { SpecimenAPI } from '../APIs';
import { useBiobankContext } from '../hooks';
import Form from 'Form';
const {
  ButtonElement,
  TextboxElement,
  SelectElement,
  DateElement,
  TimeElement,
  TextareaElement,
  StaticElement,
} = Form;

type ProcessFormProps = {
  specimen?: SpecimenHook,
  process: ProcessHook,
  processStage: string,
  type: ISpecimenType,
  edit?: boolean,
  hideProtocol?: boolean,
};

function ProcessForm({
  specimen,
  process,
  processStage,
  type,
  edit,
  hideProtocol,
}: ProcessFormProps): ReactElement {
  const { options } = useBiobankContext();

  const updateButton = specimen && (
    <ButtonElement
      label="Update"
      onUserInput={ () => new SpecimenAPI().update(specimen)}
    />
  );


  // XXX: THIS IS A DYNAMICALLY GENERATED CONFIG — FIND A WAY TO PULL THIS INTO
  // ANOTHER COMPONENT NOW!!!
  let specimenProtocolAttributes = {};
  Object.entries(options.specimen?.protocols as { [key: string]: IProtocol }).forEach(([id, protocol]) => {
    // FIXME: I really don't like 'toLowerCase()' function, but it's the
    // only way I can get it to work at the moment.
    if (type.label == protocol.type.label &&
        protocol.process.toLowerCase() ==
        processStage) {
      specimenProtocolAttributes[id] = options.specimen.protocolAttributes[id];
    }
  });

  const renderProtocolFields = () => {
    if (process.protocol.attributes) {
      if (process.data) {
        return (
          <CustomFields
            attributes={process.protocol.attributes}
            data={process.data}
           />
        );
      } else {
        process.data.clear()
      }
    }
  };

  if (type && edit === true) {
    return (
      <>
        {!hideProtocol && (
          <ProcessField property={'protocol'} process={process}/>
        )}
        <ProcessField property={'examiner'} process={process}/>
        <ProcessField property={'date'} process={process}/>
        <ProcessField property={'time'} process={process}/>
        {processStage === 'collection' && (
          <>
            <ProcessField property={'quantity'} process={process}/>
            <ProcessField property={'unit'} process={process}/>
          </>
        )}
        <div className='form-top'/>
        {renderProtocolFields()}
        <ProcessField property={'comments'} process={process}/>
        {updateButton}
      </>
    );
  } else if (edit === false) {
    const protocolStaticFields = process.data &&
      Object.keys(process.data).map((key) => {
        let value = process.data[key];
        if (process.data[key] === true) {
          value = 'Yes';
        } else if (process.data[key] === false) {
          value = 'No';
        }
        // FIXME: The label used to be produced in the following way:
        // label={options.specimen.protocolAttributes[process.protocolId][key].label}
        // However, causes issues when there is data in the data
        // object, but the protocolId is not associated with any attributes.
        // This is a configuration/importing issue that should be fixed.
        return (
          <StaticElement
            key={key}
            label={options.specimen.attributes[key].label}
            text={value}
          />
        );
      });

    const collectionStaticFields = (processStage === 'collection') && (
      <StaticElement label='Quantity' text={process.quantity+' '+process.unit}/>
    );

    return (
      <>
        <StaticElement label='Protocol' text={process.protocol}/>
        <StaticElement label='Site' text={process.center}/>
        <StaticElement label='Done By' text={process.examiner}/>
        <StaticElement label='Date' text={process.date} />
        <StaticElement label='Time' text={process.time} />
        {collectionStaticFields}
        {protocolStaticFields}
        <StaticElement label='Comments' text={process.comments} />
      </>
    );
  }

  return null;
};

export default ProcessForm;
