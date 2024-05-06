import React, { useState, useEffect, ReactElement, useRef} from 'react';
import { ContainerParentForm, SpecimenField, ContainerField, ProcessForm,
  ListForm } from '../components';
import Modal from 'Modal';
import { mapFormOptions, clone, padBarcode } from '../utils';
import { useBiobankContext } from '../hooks';
import { Options } from '../types';
import {
  ISpecimen,
  Specimen,
  SpecimenHook,
  SpecimenProvider,
  useSpecimen,
  useContainer,
  useEntities,
  EntitiesHook,
} from '../entities';
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

type SpecimenFormProps = {
  parent?: SpecimenHook,
  show: boolean,
  onClose: () => void,
  title: string,
};

function SpecimenForm({
  parent,
  show,
  onClose,
  title,
}: SpecimenFormProps): ReactElement {

  const { options, containers } = useBiobankContext();

  const specimen = useSpecimen();
  const container = useContainer();
  const specimens = useEntities<Specimen, ISpecimen>(Specimen);
  const [printBarcodes, setPrintBarcodes] = useState(false);

  useEffect(() => {
    
    // specimen.set('parentSpecimens', Object.values(parent).map((item) => item.specimen.id));
    if (parent) {
      specimen.set('candidate', parent.candidate);
      specimen.set('session', parent.session);
      specimen.set('type', parent.type);
      specimen.container.set('center', parent.container.center);
    }


    // if (parent.length > 1) {
    //   specimen.set('quantity', 0)
    // }
  }, [parent]); // Dependency array, effect runs when `parent` changes

  /**
   * When a session is selected, set the session, center
   *
   * @param {object} session
   * @param {string} session
   */
  const setSession = (session, sessionId) => {
    container.set('center', options.sessionCenters[sessionId].center); 
    specimen.set('session', session);
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
    // const pscid = specimen.candidate;
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

  const increaseCoordinate = (coordinate, parentContainer) => {               
    const containers = {}; // TODO: fill this with actual containers to make it work!  
    const childCoordinates =
      containers[parentContainer].childContainers    
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
  //   const errors = {specimen: {}, container: {}, list: {}};                  
                                                                                
  //   let isError = false;                                                     
  //   Object.keys(list).reduce((coord, key) => {                               
  //     // set specimen values                                                 
  //     const newSpecimen = list[key];                                         
  //     newSpecimen.candidate = specimen.candidate;                        
  //     newSpecimen.session = specimen.session;                            
  //     newSpecimen.projects = specimen.projects;                          
  //     newSpecimen.quantity = newSpecimen.collection.quantity;                
  //     newSpecimen.unit = newSpecimen.collection.unit;                    
  //     newSpecimen.collection.center = container.center;                  
  //     if ((options.specimen.types[newSpecimen.type]||{}).freezeThaw == 1) {
  //       newSpecimen.fTCycle = 0;                                             
  //     }                                                                      
  //     newSpecimen.parentSpecimens = specimen.parentSpecimens || null;    
                                                                                
  //     // set container values                                                
  //     const newContainer = newSpecimen.container;                            
  //     newContainer.temperature = 20;                                         
  //     newContainer.center = container.center;                            
                                                                                
  //     // If the container is assigned to a parent, place it sequentially in the
  //     // parent container and inherit the status, temperature and center.  
  //     if (newContainer.parentContainerBarcode) {                                  
  //       const containerParentBarcode = newContainer.parentContainerBarcode;            
  //       newContainer.parentContainerBarcode =
    //       container.parentContainerBarcode;        
  //       const parentContainer = containers[containerParentBarcode];               
  //       const dimensions = parentContainer.dimensions;                
  //       const capacity = dimensions.x * dimensions.y * dimensions.z;         
  //       coord = increaseCoordinate(                                          
  //          coord,                                                            
  //          container.parentContainerBarcode                                      
  //       );                                                                   
  //       if (coord <= capacity) {                                             
  //         newContainer.coordinate = parseInt(coord);                         
  //       } else {                                                             
  //         newContainer.coordinate = null;                                    
  //       }                                                                    
  //       newContainer.status = parentContainer.status;                    
  //       newContainer.temperature = parentContainer.temperature;              
  //       newContainer.center = parentContainer.center;                    
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
    if (parent && specimen.candidate && specimen.session) {
      const parentBarcodes = Object.values(parent).map(
        (item) => item.barcode
      );
      const parentBarcodesString = parentBarcodes.join(', ');
      return (
        <div>
          <StaticElement
            label="Parent Specimen(s)"
            text={parentBarcodesString}/>
          <SpecimenField property={'candidate'} isStatic/>
          <SpecimenField property={'session'} isStatic/>
        </div>
      );
    } else {
      return (
        <>
          <SpecimenField property='candidate'/>
          <SpecimenField property='session'/>
        </>
      );
    }
  };

  const renderRemainingQuantityFields = () => {
    if (parent) {
      // if (loris.userHasPermission('biobank_specimen_update')
      //      && parent.length === 1) {
        return (
          <>
            <SpecimenField property={'quantity'}/>
            <SpecimenField property={'unit'}/>
          </>
        );
      // }
    }
  };

  // Assuming container.parentContainerBarcode is defined
  if (container.parentContainer) {
      let initialCoord = 0; // Initial coordinate value
      let coordinates = []; // Local array to accumulate coordinates
  
      for (const key of Object.keys(specimens)) {
          initialCoord = increaseCoordinate(
              initialCoord,
              container.parentContainer,
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
      <SpecimenProvider specimen={specimen}>
        <div className='row'>
          <div className="col-xs-11">
            <StaticElement
              label='Note'
              text={parent ? dict.noteForAliquots : dict.noteForSpecimens}
            />
            {renderGlobalFields()}
            <SelectElement
              name='projects'
              label='Project'
              options={options.projects}
              onUserInput={specimen.set}
              required={true}
              value={specimen.projects}
              disabled={specimen.candidate ? false : true}
              errorMessage={specimen.errors.projects}
            />
            {renderRemainingQuantityFields()}
          </div>
        </div>
        <ListForm list={specimens} listItemComponent={SpecimenBarcodeForm}/>
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
          disabled={specimen.candidate ? false : true}
        />
        <CheckboxElement
          name='printBarcodes'
          label='Print Barcodes'
          onUserInput={(name, value) => setPrintBarcodes(value)}
          value={printBarcodes}
        />
      </SpecimenProvider>
    </Modal>
  );
}

const SpecimenBarcodeForm: React.FC<{
  id: string,
  entity: Specimen,
  update: EntitiesHook<Specimen, ISpecimen>['update'],
}> = ({
  id,
  entity,
  update,
}) => {

  const specimen = useSpecimen(entity.getData());

  useEffect(() => {
    update(id, specimen.getData());
  }, [specimen]);

  return (
    <SpecimenProvider specimen={specimen}>
      <ContainerField property={'barcode'}/>
      <SpecimenField property={'type'}/>
      <ContainerField property={'type'}/>
      <ContainerField property={'lotNumber'}/>
      <ContainerField property={'expirationDate'}/>
      <ProcessForm
        edit={true}
        process={specimen.collection}
        processStage='collection'
        type={specimen.type}
      />
    </SpecimenProvider>
  );
}

export default SpecimenForm;
