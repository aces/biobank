import React, { useState, useCallback, useEffect } from 'react';
import Modal from 'Modal';
import { ListForm, ContainerField } from '../forms';
import { IContainer, Container, useEntities, EntitiesHook, ContainerProvider, useContainer } from '../entities';
import { useBiobankContext} from '../hooks';
import { ContainerAPI } from '../APIs';

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

  return (
    <Modal
      title='Add New Container'
      show={show}
      onClose={onClose}
      onSubmit={() => new ContainerAPI().create(containers)}
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
