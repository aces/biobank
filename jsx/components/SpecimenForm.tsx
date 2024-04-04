import React, { useState, useEffect, ReactElement, useRef} from 'react';
import { ContainerParentForm, SpecimenField, ContainerField, ProcessForm, ListForm2 } from '../components';
import Modal from 'Modal';
import { mapFormOptions, clone, padBarcode } from '../utils';
import { useSpecimen, useContainer, useBiobankContext, useEntities} from '../hooks';
import { Container, Specimen, Options, Process } from '../types';
import Form from 'Form';
const {
  CheckboxElement,
  SelectElement,
  StaticElement,
  TextboxElement,
  FormElement,
  ButtonElement,
  DateElement,
  SearchableDropdown,
} = Form;
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
  parent?: [{ specimen: Specimen, container: Container }], // Replace ParentType with the correct type
  show: boolean,
  onClose: () => void,
  setSpecimen?: Function, //TODO: type
  title: string,
};

function SpecimenForm({
  parent,
  show,
  onClose,
  setSpecimen,
  title,
}: SpecimenFormProps): ReactElement {

  const { options, containers } = useBiobankContext();

  const specimen = useSpecimen({});
  const container = useContainer({});
  const specimens = useEntities<Specimen>();
  const [printBarcodes, setPrintBarcodes] = useState<boolean>(false);

  const prevParent = usePrevious(parent);

  useEffect(() => {
    // This code will run when the component mounts and whenever `props.parent` changes
    if (parent !== prevParent) { // You need to track prevParent, see below
      const parentSpecimen = parent[0].specimen;
      const parentContainer = parent[0].container;
      
      specimen.set('parentSpecimenIds', Object.values(parent).map((item) =>
item.specimen.id));
      specimen.set('candidateId', parentSpecimen.candidateId);
      specimen.set('sessionId', parentSpecimen.sessionId);
      specimen.set('typeId', parentSpecimen.typeId);
      container.set('centerId', parentContainer.centerId);


      if (parent.length > 1) {
        specimen.set('quantity', 0)
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
    container.set('centerId', options.sessionCenters[sessionId].centerId); 
    specimen.set('sessionId', sessionId);
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
    // const tryIncrement = increment + 1;
    // const barcode = padBarcode(pscid, tryIncrement);

    // // TODO: remove 'as Record<string, Container>'
    // // This should already be defined in BiobankContext
    // // I'm not sure where it doesn't understand that containers is typed that
    // // way.
    // const barcodeExists = Object.values(containers as Record<string, Container>)
    // .some(container => container.barcode === barcode) ||
    //                       Object.values(specimens).some(specimen => specimen.container.barcode === barcode);

    // if (barcodeExists) {
    //   return incrementBarcode(pscid, tryIncrement);
    // }

    // return tryIncrement;
    return 0;
  };

  /**
   * Generate barcodes and store in the component state.
   */
  const generateBarcodes = () => {
    // const pscid = options.candidates[specimen.candidateId].pscid;
    // let increment = 0; // Initialize increment to 0

    // // Create a new Map from the existing list to avoid mutating the state directly
    // const newList = new Map(specimens); // Assuming 'list' is already a Map

    // newList.forEach((specimenItem, key) => {
    //   if (!specimenItem.container.barcode) {
    //     increment = incrementBarcode(pscid, increment); // Update increment
    //     const barcode = padBarcode(pscid, increment);
    //     // Update the container barcode for this specimenItem
    //     newList.set(key, { ...specimenItem, container: { ...specimenItem.container, barcode } });
    //   }
    // });

    // setList(newList); // Update state with the new Map
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

  const onSubmit = (specimens, specimen, container, print) => {               
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
    onSubmit(specimens, specimen, container, printBarcodes);
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
            onUserInput={specimen.set}
            required={true}
            value={specimen.candidateId}
            placeHolder='Search for a PSCID'
            errorMessage={specimen.errors.candidateId}
          />
          <SelectElement
            name='sessionId'
            label='Visit Label'
            options={sessions}
            onUserInput={setSession}
            required={true}
            value={specimen.sessionId}
            disabled={specimen.candidateId ? false : true}
            errorMessage={specimen.errors.sessionId}
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
        return (
          <>
            <SpecimenField field={'quantity'} specimen={specimen}/>
            <SpecimenField field={'unitId'} specimen={specimen}/>
          </>
        );
      }
    }
  };

  // Assuming container.parentContainerId is defined
  if (container.parentContainerId) {
      let initialCoord = 0; // Initial coordinate value
      let coordinates = []; // Local array to accumulate coordinates
  
      for (const key of Object.keys(specimens)) {
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
      container.set('coordinate', coordinates);
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
              onUserInput={specimen.set}
              required={true}
              value={specimen.projectIds}
              disabled={specimen.candidateId ? false : true}
              errorMessage={specimen.errors.projectIds}
            />
            {renderRemainingQuantityFields()}
          </div>
        </div>
        <ListForm2 list={specimens}>
          <SpecimenBarcodeForm/>
        </ListForm2>
        <br/>
        <div className='form-top'/>
        <ContainerParentForm
          container={container}
          display={true}
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

const SpecimenBarcodeForm: React.FC<{
  entity: Specimen,
}> = ({
  entity: specimen,
}) => {

  const { options } = useBiobankContext();

  // XXX: Only allow the selection of child types
  const renderSpecimenTypes = () => {
    let specimenTypes;
    if (specimen.typeId) {
      specimenTypes = Object.entries(options.specimen.types).reduce(
        (result, [id, type]) => {
          if (id == specimen.typeId) {
            result[id] = type;
          }

          if (type.parentTypeIds) {
            type.parentTypeIds.forEach((i) => {
              if (i == specimen.typeId) {
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
  if (specimen.typeId && options.specimen.typeContainerTypes[specimen.typeId]) {
    Object.keys(containerTypesPrimary).forEach((id) => {
      options.specimen.typeContainerTypes[specimen.typeId].forEach((i) => {
        if (id == i) {
          validContainers[id] = containerTypesPrimary[id];
        }
      });
    });
  }

  return (
    <>
      <ContainerField property={'barcode'} container={specimen.container}/>
      <SelectElement
        name="typeId"
        label="Specimen Type"
        options={renderSpecimenTypes()}
        onUserInput={specimen.set}
        required={true}
        value={specimen.typeId}
        errorMessage={(specimen.errors.specimen||{}).typeId}
      />
      <SelectElement
        name="typeId"
        label="Container Type"
        options={specimen.typeId ? validContainers : containerTypesPrimary}
        onUserInput={specimen.container.set}
        required={true}
        value={specimen.container.typeId}
        errorMessage={specimen.errors.container?.typeId}
        autoSelect={true}
      />
      <ContainerField property={'lotNumber'} container={specimen.container}/>
      <ContainerField property={'expirationDate'} container={specimen.container}/>
      <ProcessForm
        edit={true}
        errors={errors}
        process={specimen.collection}
        processStage='collection'
        typeId={specimen.typeId}
      />
    </>
  );
}

export default SpecimenForm;
