import React, { useState } from 'react';
import Modal from 'Modal';
import { ListForm2, ContainerField } from '../components';
import { Container } from '../types';
import { isEmpty, clone, mapFormOptions } from '../utils';
import { useEntities, useBiobankContext, useContainer} from '../hooks';
import { ContainerAPI } from '../APIs';
import Form from 'Form';
const {
  FormElement,
} = Form;

/**
 * A form for editing Containers in the Biobank
 */
const ContainerForm: React.FC<{
  show: boolean,
  onClose: () => void,
}> = ({
  show = false,
  onClose
}) => {
  const containers = useEntities<Container>();
  const container = useContainer();
  const { options } = useBiobankContext();

  const onSubmit = async (errors) => {                         
    const availableId = Object.keys(options.container.stati)                                   
    .find(key => options.container.stati[key].label === 'Available');                        
                                                                             
    containers.setAll('statusId', availableId);
    containers.setAll('temperature', 20);
    containers.setAll('centerId', container.centerId);

    if (!containers.validateAll()) {                                                           
      return;                                         
    }                                                                        
                                                                             
    try {
      const savedContainers = await ContainerAPI.post(containers.toArray());
      // Assuming setContainers is meant to update the state with the saved containers
    } catch (error) {
      console.error("Failed to save containers:", error);
      throw error;
    }
  } 

  return (
    <Modal
      title='Add New Container'
      show={show}
      onClose={onClose}
      onSubmit={onSubmit}
      throwWarning={true}
    >
      <FormElement>
        <div className="row">
          <div className="col-xs-11">
            <ContainerField property={'center'} container={container}/>
          </div>
        </div>
        <ListForm2 list={containers}>
          <ContainerSubForm/>
        </ListForm2>
      </FormElement>
    </Modal>
  );
}

const ContainerSubForm: React.FC<{
  entity: Container,
}> = ({
  entity: container,
}) => {
  return (
    <>
      <ContainerField property={'barcode'} container={container}/>
      <ContainerField property={'type'} container={container}/>
      <ContainerField property={'lotNumber'} container={container}/>
      <ContainerField property={'expirationDate'} container={container}/>
    </>
  );
}

export default ContainerForm;
