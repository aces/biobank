import React, { useState, useCallback, useEffect } from 'react';
import Modal from 'Modal';
import { ListForm, ContainerField } from '../components';
import { IContainer, Container, useEntities, EntitiesHook, ContainerProvider, useContainer } from '../entities';
import { useBiobankContext} from '../hooks';
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
  const containers = useEntities(Container);
  const container = useContainer();
  const { options } = useBiobankContext();
  console.log(containers);

  const onSubmit = async (errors) => {                         
    containers.setAll('temperature', 20);
    containers.setAll('center', container.center);

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
      <ContainerField property={'center'}/>
      <ListForm list={containers} listItemComponent={ContainerListItem}/>
    </Modal>
  );
}

// XXX: I have stacked this with memoization — not sure how much is necessary
// but for now it works!
const ContainerListItem: React.FC<{
  id: string,
  entity: Container,
  update: EntitiesHook<Container, IContainer>['update']
}> = React.memo(({ id, entity, update }) => {

  const container = useContainer(entity.getData());

  // Memoize callback
  const handleUpdate = useCallback(() => {
    update(id, container.getData());
  }, [id, container.getData(), update]);

  // Effect runs only once or when explicitly needed
  useEffect(() => {
    handleUpdate();
  }, [handleUpdate]);

  return (
    <ContainerProvider container={container}>
      <ContainerField property={'barcode'}/>
      <ContainerField property={'type'}/>
      <ContainerField property={'lotNumber'}/>
      <ContainerField property={'expirationDate'}/>
    </ContainerProvider>
  );
})

export default ContainerForm;
