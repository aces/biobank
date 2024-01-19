import { ReactElement } from 'react';
import { mapFormOptions, clone } from '../utils';
import { CustomFields } from '../components';
import { Options, Process, Specimen, SpecimenHandler } from '../types';
import {
  ButtonElement,
  TextboxElement,
  SelectElement,
  DateElement,
  TimeElement,
  TextareaElement,
  StaticElement,
} from 'Form';

type ProcessFormProps = {
  specimen?: Specimen,
  process: Process,
  processStage: string,
  typeId: string,
  options: Options,
  errors: any, //TODO: type
  edit?: boolean,
  setParent: (processStage: string, process: Process) => void,
  hideProtocol?: boolean,
  updateSpecimen?: (url: string) => Promise<void>,
};

function ProcessForm({
  specimen,
  updateSpecimen,
  process,
  processStage,
  typeId,
  options,
  errors,
  edit,
  setParent,
  hideProtocol,
}: ProcessFormProps): ReactElement {
  const setProcess = (name, value) => {
    let processClone = clone(process);
    processClone[name] = value;
    setParent(processStage, processClone);
  };

  const setProtocol = (name, value) => {
    let processClone = clone(process);
    processClone[name] = value;
    processClone.data = {};
    setParent(processStage, processClone);
  };

  const setData = (name, value) => {
    const data = clone(process.data);
    // TODO: will have to rejig file saving — likely to be turned into a state
    if (value instanceof File) {
      data[name] = value.name;
      // setFiles(prevFiles => { ...prevFiles, [value.name]: value});
    } else {
      data[name] = value;
    }
    setProcess('data', data);
  };

  const updateButton = specimen && (
    <ButtonElement
      label="Update"
      onUserInput={updateSpecimen}
    />
  );

  let specimenProtocols = {};
  let specimenProtocolAttributes = {};
  Object.entries(options.specimen?.protocols).forEach(([id, protocol]) => {
    // FIXME: I really don't like 'toLowerCase()' function, but it's the
    // only way I can get it to work at the moment.
    if (typeId == protocol.typeId &&
        options.specimen.processes[protocol.processId].label.toLowerCase() ==
        processStage) {
      specimenProtocols[id] = protocol.label;
      specimenProtocolAttributes[id] = options.specimen.protocolAttributes[id];
    }
  });

  const renderProtocolFields = () => {
    if (specimenProtocolAttributes[process.protocolId]) {
      if (process.data) {
        return (
          <CustomFields
            options={options}
            errors={errors.data || {}}
            fields={specimenProtocolAttributes[process.protocolId]}
            data={process.data}
            setData={setData} 
           />
        );
      } else {
        setProcess('data', {});
      }
    }
  };

  const specimenTypeUnits = Object.keys(options.specimen.typeUnits[typeId]||{})
  .reduce((result, id) => {
    result[id] = options.specimen.typeUnits[typeId][id].label;
    return result;
  }, {});
  const collectionFields = processStage === 'collection' && [
    <TextboxElement
      name="quantity"
      label="Quantity"
      onUserInput={setProcess}
      required={true}
      value={process.quantity}
      errorMessage={errors.quantity}
    />,
    <SelectElement
      name="unitId"
      label="Unit"
      options={specimenTypeUnits}
      onUserInput={setProcess}
      required={true}
      value={process.unitId}
      errorMessage={errors.unitId}
      autoSelect={true}
    />,
  ];

  const protocolField = !hideProtocol && (
    <SelectElement
      name="protocolId"
      label="Protocol"
      options={specimenProtocols}
      onUserInput={setProtocol}
      required={true}
      value={process.protocolId}
      errorMessage={errors.protocolId}
      autoSelect={true}
    />
  );

  const examiners = mapFormOptions(options.examiners, 'label');
  if (typeId && edit === true) {
    return (
      <>
        {protocolField}
        <SelectElement
          name="examinerId"
          label="Done By"
          options={examiners}
          onUserInput={setProcess}
          required={true}
          value={process.examinerId}
          errorMessage={errors.examinerId}
          autoSelect={true}
        />
        <DateElement
          name="date"
          label="Date"
          onUserInput={setProcess}
          required={true}
          value={process.date}
          errorMessage={errors.date}
        />
        <TimeElement
          name="time"
          label="Time"
          onUserInput={setProcess}
          required={true}
          value={process.time}
          errorMessage={errors.time}
        />
        {collectionFields}
        <div className='form-top'/>
        {renderProtocolFields()}
        <TextareaElement
          name="comments"
          label="Comments"
          onUserInput={setProcess}
          value={process.comments}
          errorMessage={errors.comments}
        />
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
      <StaticElement
        label='Quantity'
        text={process.quantity+' '+options.specimen.units[process.unitId].label}
      />
    );

    return (
      <>
        <StaticElement
          label='Protocol'
          text={options.specimen.protocols[process.protocolId].label}
        />
        <StaticElement
          label='Site'
          text={options.centers[process.centerId]}
        />
        <StaticElement
          label='Done By'
          text={options.examiners[process.examinerId].label}
        />
        <StaticElement
          label='Date'
          text={process.date}
        />
        <StaticElement
          label='Time'
          text={process.time}
        />
        {collectionStaticFields}
        {protocolStaticFields}
        <StaticElement
          label='Comments'
          text={process.comments}
        />
      </>
    );
  }

  return null;
};

export default ProcessForm;
