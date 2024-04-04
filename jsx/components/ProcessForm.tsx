import { ReactElement } from 'react';
import { mapFormOptions, clone } from '../utils';
import { CustomFields } from '../components';
import { Protocol, Process, Specimen, SpecimenHandler } from '../types';
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
  specimen?: Specimen,
  process: Process,
  processStage: string,
  typeId: string,
  errors?: any, //TODO: type + figure out if mandatory
  edit?: boolean,
  hideProtocol?: boolean,
};

function ProcessForm({
  specimen,
  process,
  processStage,
  typeId,
  errors,
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
  Object.entries(options.specimen?.protocols as { [key: string]: Protocol }).forEach(([id, protocol]) => {
    // FIXME: I really don't like 'toLowerCase()' function, but it's the
    // only way I can get it to work at the moment.
    if (typeId == protocol.typeId &&
        options.specimen.processes[protocol.processId].label.toLowerCase() ==
        processStage) {
      specimenProtocolAttributes[id] = options.specimen.protocolAttributes[id];
    }
  });

  const renderProtocolFields = () => {
    if (specimenProtocolAttributes[process.protocolId]) {
      if (process.data) {
        return (
          <CustomFields
            fields={specimenProtocolAttributes[process.protocolId]}
            data={process.data}
           />
        );
      } else {
        setProcess('data', {});
      }
    }
  };

  if (typeId && edit === true) {
    return (
      <>
        {!hideProtocol && (
          <ProcessField property={'protocolId'} process={process}/>
        )}
        <ProcessField property={'examinerId'} process={process}/>
        <ProcessField property={'date'} process={process}/>
        <ProcessField property={'time'} process={process}/>
        {collectionFields}
        {processState === 'collection' && (
          <>
            <ProcessField property={'quantity'} process={process}/>
            <ProcessField property={'unitId'} process={process}/>
          </>
        )
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
      <StaticElement label='Quantity' text={process.quantity+' '+options.specimen.units[process.unitId].label}/>
    );

    return (
      <>
        <StaticElement label='Protocol' text={options.specimen.protocols[process.protocolId].label}/>
        <StaticElement label='Site' text={options.centers[process.centerId]}/>
        <StaticElement label='Done By' text={options.examiners[process.examinerId].label}/>
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
