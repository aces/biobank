import React, { useState, useCallback, useEffect } from 'react';
import Modal from 'Modal';
import { ListForm, ContainerField } from '../forms';
import { IContainer, Container, useEntities, EntitiesHook, ContainerProvider, useContainer } from '../entities';
import { useBiobankContext} from '../hooks';
import { ContainerAPI } from '../APIs';

const ContainerForm: React.FC<{
  show: boolean,
  onClose: () => void,
}> = ({
  show = false,
  onClose
}) => {
  if (!show) return null; //XXX temporary until Modal does this properly.

  const container = useContainer(new Container({}));
  const containers = useEntities(Container);

  useEffect(() => {
    containers.add({});
  }, [containers.add]);                                 

  useEffect(() => {
    containers.setAll('center', {label: container.center});
  }, [container.center, containers.entities.size]);                                 

  const onSubmit = () => {
    return containers.saveAll(data => new ContainerAPI().create(data));
  };

  return (
    <Modal
      title='Add New Container'
      show={show}
      onClose={onClose}
      onSubmit={onSubmit}
      throwWarning={true}
    >
      <ContainerProvider container={container}>
        <ContainerField property={'center'}/>
      </ContainerProvider>
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

  const container = useContainer(entity);

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
