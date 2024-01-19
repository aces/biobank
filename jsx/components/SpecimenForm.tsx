import React, { useState, useEffect, ReactElement, useRef} from 'react';
import { ContainerParentForm, ProcessForm, ListForm, ListItem } from '../components';
import Modal from 'Modal';
import { mapFormOptions, clone, padBarcode } from '../utils';
import { useSpecimen, useContainer } from '../hooks';
import { Data, Container, Specimen, Options, Process } from '../types';
import {
  CheckboxElement,
  SelectElement,
  StaticElement,
  TextboxElement,
  FormElement,
  ButtonElement,
  DateElement,
  SearchableDropdown,
} from 'Form';
import dict from '../i18n/en.json';
declare const loris: any;

// To track the previous value of props.parent
const usePrevious = (value) => {
  const ref = useRef();
  useEffect(() => {
    ref.current = value;
  });
  return ref.current;
};

type SpecimenFormProps = {
  parent: [{ specimen: Specimen, container: Container }], // Replace ParentType with the correct type
  options: Options,
  data: Data, 
  show: boolean,
  onClose: Function,
  setSpecimen: Function, //TODO: type
  title: string,
};

function SpecimenForm({
  parent,
  options,
  data,
  show,
  onClose,
  setSpecimen,
  title,
}: SpecimenFormProps): ReactElement {

  const [specimen, specHandler] = useSpecimen({});
  const [container, contHandler] = useContainer({});
  const [list, setList] = useState<Record<string, Specimen>>({}); //TODO: LIST SHOULD LIKELY BE AN ARRAY
  const [printBarcodes, setPrintBarcodes] = useState<boolean>(false);
  const [errors, setErrors] = useState({list: {}}); //TODO: type

  const prevParent = usePrevious(parent);

  useEffect(() => {
    // This code will run when the component mounts and whenever `props.parent` changes
    if (parent !== prevParent) { // You need to track prevParent, see below
      const parentSpecimen = parent[0].specimen;
      const parentContainer = parent[0].container;
      
      specHandler.set('parentSpecimenIds', Object.values(parent).map((item) =>
item.specimen.id));
      specHandler.set('candidateId', parentSpecimen.candidateId);
      specHandler.set('sessionId', parentSpecimen.sessionId);
      specHandler.set('typeId', parentSpecimen.typeId);
      contHandler.set('centerId', parentContainer.centerId);


      if (parent.length > 1) {
        specHandler.set('quantity', 0)
      }
    }
  }, [parent]); // Dependency array, effect runs when `parent` changes

  /**
   * When a session is selected, set the sessionId, centerId
   *
   * @param {object} session
   * @param {string} sessionId
   */
  const setSession = (session, sessionId) => {
    contHandler.set('centerId', options.sessionCenters[sessionId].centerId); 
    specHandler.set('sessionId', sessionId);
  }

  /**
   * Increment the current barcode
   *
   * @param {string} pscid - the PSCID
   * @param {number} increment - the amount to increment
   *
   * @return {number}
   */
  const incrementBarcode = (pscid: string, increment: number = 0): number => {
    increment++;
    const barcode = padBarcode(pscid, increment);
    if (Object.values(data.containers)
         .some((container) => container.barcode === barcode)) {
      increment = incrementBarcode(pscid, increment);
    }
    if (Object.values(list)
         .some((specimen) => specimen.container.barcode === barcode)) {
      increment = incrementBarcode(pscid, increment);
    }
    return increment;
  };

  /**
   * Generate barcodes and store in the component state.
   */
  const generateBarcodes = () => {
    const pscid = options.candidates[specimen.candidateId].pscid;
    let increment = 0; // Initialize increment to 0
  
    const newList = Object.keys(list).reduce((result, key) => {
      const specimenItem = list[key];
      if (!specimenItem.container.barcode) {
        increment = incrementBarcode(pscid, increment); // Update increment
        const barcode = padBarcode(pscid, increment);
        specimenItem.container = { ...specimenItem.container, barcode };
      }
      result[key] = specimenItem;
      return result;
    }, {});
  
    setList(newList);
  };

  const increaseCoordinate = (coordinate, parentContainerId) => {               
    const containers = {}; // TODO: fill this with actual containers to make it work!                                              
    const childCoordinates = containers[parentContainerId].childContainerIds    
    .reduce((result, id) => {                                                   
      const container = containers[id];                                         
      if (container.coordinate) {                                               
        result[container.coordinate] = id;                                      
      }                                                                         
      return result;                                                            
    }, {});                                                                     
                                                                                
    const increment = (coord) => {                                              
      coord++;                                                                  
      if (childCoordinates.hasOwnProperty(coord)) {                             
        coord = increment(coord);                                               
      }                                                                         
                                                                                
      return coord;                                                             
    };                                                                          
                                                                                
    return increment(coordinate);                                               
  }   

  const onSubmit = (list, specimen, container, print) => {               
  //   const labelParams = [];                                                  
  //   const availableId = Object.keys(options.container.stati).find(           
  //     (key) => options.container.stati[key].label === 'Available'            
  //   );                                                                       
  //   const errors = {specimen: {}, container: {}, list: {}};                  
                                                                                
  //   let isError = false;                                                     
  //   Object.keys(list).reduce((coord, key) => {                               
  //     // set specimen values                                                 
  //     const newSpecimen = list[key];                                         
  //     newSpecimen.candidateId = specimen.candidateId;                        
  //     newSpecimen.sessionId = specimen.sessionId;                            
  //     newSpecimen.projectIds = specimen.projectIds;                          
  //     newSpecimen.quantity = newSpecimen.collection.quantity;                
  //     newSpecimen.unitId = newSpecimen.collection.unitId;                    
  //     newSpecimen.collection.centerId = container.centerId;                  
  //     if ((options.specimen.types[newSpecimen.typeId]||{}).freezeThaw == 1) {
  //       newSpecimen.fTCycle = 0;                                             
  //     }                                                                      
  //     newSpecimen.parentSpecimenIds = specimen.parentSpecimenIds || null;    
                                                                                
  //     // set container values                                                
  //     const newContainer = newSpecimen.container;                            
  //     newContainer.statusId = availableId;                                   
  //     newContainer.temperature = 20;                                         
  //     newContainer.centerId = container.centerId;                            
                                                                                
  //     // If the container is assigned to a parent, place it sequentially in the
  //     // parent container and inherit the status, temperature and centerId.  
  //     if (newContainer.parentContainerId) {                                  
  //       const containerParentId = newContainer.parentContainerId;            
  //       newContainer.parentContainerId = container.parentContainerId;        
  //       const parentContainer = containers[containerParentId];               
  //       const dims = options.container.dimensions;                           
  //       const dimensions = dims[parentContainer.dimensionId];                
  //       const capacity = dimensions.x * dimensions.y * dimensions.z;         
  //       coord = increaseCoordinate(                                          
  //          coord,                                                            
  //          container.parentContainerId                                       
  //       );                                                                   
  //       if (coord <= capacity) {                                             
  //         newContainer.coordinate = parseInt(coord);                         
  //       } else {                                                             
  //         newContainer.coordinate = null;                                    
  //       }                                                                    
  //       newContainer.statusId = parentContainer.statusId;                    
  //       newContainer.temperature = parentContainer.temperature;              
  //       newContainer.centerId = parentContainer.centerId;                    
  //     }                                                                      
                                                                                
  //     // if specimen type id is not set yet, this will throw an error        
  //     if (newSpecimen.typeId) {                                              
  //       labelParams.push({                                                   
  //         barcode: newContainer.barcode,                                     
  //         type: options.specimen.types[newSpecimen.typeId].label,            
  //       });                                                                  
  //     }                                                                      
                                                                                
  //     newSpecimen.container = newContainer;                                  
  //     list[key] = newSpecimen;                                               
                                                                                
  //     // this is so the global params (sessionId, candidateId, etc.) show errors
  //     // as well.                                                            
  //     // errors.container = this.validateContainer(newContainer, key);       
  //     // errors.specimen = this.validateSpecimen(newSpecimen, key);          
                                                                                
  //     if (!isEmpty(errors.container)) {                                      
  //       errors.list[key] = {container: errors.container};                    
  //     }                                                                      
  //     if (!isEmpty(errors.specimen)) {                                       
  //       errors.list[key] = {...errors.list[key], specimen: errors.specimen}; 
  //     }                                                                      
                                                                                
  //     if (!isEmpty(errors.list[key])) {                                      
  //       isError = true;                                                      
  //     }                                                                      
                                                                                
  //     return coord;                                                          
  //   }, 0);                                                                   
                                                                                
  //   if (isError) {                                                           
  //     return Promise.reject(errors);                                         
  //   }                                                                        
                                                                                
  //   const printBarcodes = () => {                                            
  //     return new Promise((resolve) => {                                      
  //       if (print) {                                                         
  //         Swal.fire({                                                        
  //           title: 'Print Barcodes?',                                        
  //           type: 'question',                                                
  //           confirmButtonText: 'Yes',                                        
  //           cancelButtonText: 'No',                                          
  //           showCancelButton: true,                                          
  //         })                                                                 
  //         .then((result) => result.value && printLabel(labelParams))         
  //         .then(() => resolve());                                            
  //       } else {                                                             
  //         resolve();                                                         
  //       }                                                                    
  //     });                                                                    
  //   };                                                                       
                                                                                
  //   return printBarcodes()                                                   
  //   .then(() => post(list, specimenAPI, 'POST'))                             
  //   .then((entities) => {                                                    
  //     setContainers(entities.containers);                                    
  //     setSpecimen(entities.specimens);                                       
  //   })                                                                       
  //   .then(() => Promise.resolve());                                          
   } 


  /**
  * Handle the submission of a form
  *
  * @return {Promise}
  */
  // TODO: THIS WILL LIKELY NEED TO BE COMPLETELY REDONE
  const handleSubmit = () => {
    onSubmit(list, specimen, container, printBarcodes);
  }

  const renderGlobalFields = () => {
    if (parent && specimen.candidateId && specimen.sessionId) {
      const parentBarcodes = Object.values(parent).map(
        (item) => item.container.barcode
      );
      const parentBarcodesString = parentBarcodes.join(', ');
      return (
        <div>
          <StaticElement
            label="Parent Specimen(s)"
            text={parentBarcodesString}
          />
          <StaticElement
            label="PSCID"
            text={options.candidates[specimen.candidateId].pscid}
          />
          <StaticElement
            label="Visit Label"
            text={options.sessions[specimen.sessionId].label}
          />
        </div>
      );
    } else {
    const sessions = specimen.candidateId ?
      mapFormOptions(options.candidateSessions[specimen.candidateId], 'label')
      : {};
    const candidates = mapFormOptions(options.candidates, 'pscid');
      return (
        <div>
          <SearchableDropdown
            name="candidateId"
            label="PSCID"
            options={candidates}
            onUserInput={specHandler.set}
            required={true}
            value={specimen.candidateId}
            placeHolder='Search for a PSCID'
            errorMessage={specHandler.errors.candidateId}
          />
          <SelectElement
            name='sessionId'
            label='Visit Label'
            options={sessions}
            onUserInput={setSession}
            required={true}
            value={specimen.sessionId}
            disabled={specimen.candidateId ? false : true}
            errorMessage={specHandler.errors.sessionId}
            autoSelect={true}
          />
        </div>
      );
    }
  };

  const renderRemainingQuantityFields = () => {
    if (parent) {
      if (loris.userHasPermission('biobank_specimen_update')
           && parent.length === 1) {
        const specimenUnits = mapFormOptions(
          options.specimen.units,
          'label'
        );
        return (
          <div>
            <TextboxElement
              name="quantity"
              label="Remaining Quantity"
              onUserInput={setSpecimen}
              required={true}
              value={specimen.quantity}
            />
            <SelectElement
              name="unitId"
              label="Unit"
              options={specimenUnits}
              onUserInput={setSpecimen}
              required={true}
              value={specimen.unitId}
              autoSelect={true}
            />
          </div>
        );
      }
    }
  };

  // Assuming container.parentContainerId is defined
  if (container.parentContainerId) {
      let initialCoord = 0; // Initial coordinate value
      let coordinates = []; // Local array to accumulate coordinates
  
      for (const key of Object.keys(list)) {
          initialCoord = increaseCoordinate(
              initialCoord,
              container.parentContainerId,
          );
  
          // Parse and add the new coordinate
          coordinates.push(initialCoord);
      }
  
      // Now, update the state with the new coordinates array
      // Assuming contHandler.set is your state update function
      // TODO: see if this is necessary
      contHandler.set('coordinate', coordinates);
  }

  const handleClose = () => {
    //TODO: reset all the states
    //then
    onClose();
  }

  return (
    <Modal
      title={title}
      show={show}
      onClose={handleClose}
      onSubmit={handleSubmit}
      throwWarning={true}
    >
      <FormElement>
        <div className='row'>
          <div className="col-xs-11">
            <StaticElement
              label='Note'
              text={parent ? dict.noteForAliquots : dict.noteForSpecimens}
            />
            {renderGlobalFields()}
            <SelectElement
              name='projectIds'
              label='Project'
              options={options.projects}
              onUserInput={specHandler.set}
              required={true}
              value={specimen.projectIds}
              disabled={specimen.candidateId ? false : true}
              errorMessage={specHandler.errors.projectIds}
            />
            {renderRemainingQuantityFields()}
          </div>
        </div>
        <ListForm
          list={list}
          errors={errors.list}
          setList={setList}
          listItem={{container: {}, collection: {}}}
        >
          <SpecimenBarcodeForm
            typeId={specimen.typeId}
            options={options}
          />
        </ListForm>
        <br/>
        <div className='form-top'/>
        <ContainerParentForm
          display={true}
          data={data}
          container={container}
          contHandler={contHandler}
          options={options}
        />
        <div className='form-top'/>
        <ButtonElement
          name='generate'
          label='Generate Barcodes'
          type='button'
          onUserInput={generateBarcodes}
          disabled={specimen.candidateId ? false : true}
        />
        <CheckboxElement
          name='printBarcodes'
          label='Print Barcodes'
          onUserInput={(name, value) => setPrintBarcodes(value)}
          value={printBarcodes}
        />
      </FormElement>
    </Modal>
  );
}

type SpecimenBarcodeFormProps = {
  item?: {
    container: Container,
    typeId: string,
    collection: Process,
    [key: string]: any, // Include other keys as needed
  },
  itemKey?: string,
  setListItem?: (field: string, value: any, key: string) => void, // Replace 'any' with more specific types if possible
  options: Options,
  errors?: {
    container: { [key: string]: string }, // Include the error structure for container
    specimen: { [key: string]: string }, // Include the error structure for specimen
    [key: string]: any, // Include other error structures as needed
  },
  typeId: string,
}

function SpecimenBarcodeForm({
  item,
  itemKey,
  setListItem,
  options,
  errors,
  typeId,
  ...props
}: SpecimenBarcodeFormProps): ReactElement {
  /**
   * Set the current container.
   *
   * @param {string} name - the name
   * @param {string} value - the value
   */
  const setContainer = (name, value) => {
    let container = item.container;
    container[name] = value;
    setListItem('container', container, itemKey);
  }

  /**
   * Set the specimen
   *
   * @param {string} name - a name
   * @param {string} value - a value
   */
  const setSpecimen = (name, value) => {
    setListItem(name, value, itemKey);
  }

  // XXX: Only allow the selection of child types
  const renderSpecimenTypes = () => {
    let specimenTypes;
    if (typeId) {
      specimenTypes = Object.entries(options.specimen.types).reduce(
        (result, [id, type]) => {
          if (id == typeId) {
            result[id] = type;
          }

          if (type.parentTypeIds) {
            type.parentTypeIds.forEach((i) => {
              if (i == typeId) {
                result[id] = type;
              }
            });
          }

          return result;
        }, {}
      );
    } else {
      specimenTypes = options.specimen.types;
    }

    return mapFormOptions(specimenTypes, 'label');
  };

  const containerTypesPrimary = mapFormOptions(
    options.container.typesPrimary,
    'label',
  );

  const validContainers = {};
  if (item.typeId && options.specimen.typeContainerTypes[item.typeId]) {
    Object.keys(containerTypesPrimary).forEach((id) => {
      options.specimen.typeContainerTypes[item.typeId].forEach((i) => {
        if (id == i) {
          validContainers[id] = containerTypesPrimary[id];
        }
      });
    });
  }

  return (
    <ListItem {...props}>
      <TextboxElement
        name='barcode'
        label='Barcode'
        onUserInput={setContainer}
        required={true}
        value={item.container.barcode}
        errorMessage={(errors.container||{}).barcode}
      />
      <SelectElement
        name="typeId"
        label="Specimen Type"
        options={renderSpecimenTypes()}
        onUserInput={setSpecimen}
        required={true}
        value={item.typeId}
        errorMessage={(errors.specimen||{}).typeId}
      />
      <SelectElement
        name="typeId"
        label="Container Type"
        options={item.typeId ? validContainers : containerTypesPrimary}
        onUserInput={setContainer}
        required={true}
        value={item.container.typeId}
        errorMessage={errors.container?.typeId}
        autoSelect={true}
      />
      <TextboxElement
        name='lotNumber'
        label='Lot Number'
        onUserInput={setContainer}
        value={item.container.lotNumber}
        errorMessage={errors.container?.lotNumber}
      />
      <DateElement
        name='expirationDate'
        label='Expiration Date'
        onUserInput={setContainer}
        value={item.container.expirationDate}
        errorMessage={errors.container?.expirationDate}
      />
      <ProcessForm
        edit={true}
        errors={errors}
        options={options}
        process={item.collection}
        processStage='collection'
        setParent={setSpecimen}
        typeId={item.typeId}
      />
    </ListItem>
  );
}

export default SpecimenForm;
