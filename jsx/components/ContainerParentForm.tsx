import { ReactElement, useState, useEffect } from 'react';
import { ContainerDisplay } from '../components';
import { Container } from '../types';
import { useBiobankContext} from '../hooks';
const containerAPI = new ContainerAPI;

const ContainerParentForm: React.FC<{
  container: Container,
  display?: boolean,
}> = ({
  container,
  display
}) => {
  const { options, containers } = useBiobankContext();

  // when a parent container is selected, the passed container gets updated
  useEffect(() => {
    const parentContainer = containers[container.parentContainerId];
    if (parentContainer) {
      container.set('coordinate', null);
      container.set('temperature', parentContainer.temperature);
      container.set('centerId', parentContainer.centerId);
      container.set('statusId', parentContainer.statusId);
    }
  }, [container.parentContainerId]);

  const renderContainerDisplay = () => {
    if (!(container.parentContainerId && display)) {
      return;
    }

    return (
      <ContainerDisplay
        container={container}
        select={true}
        selectedCoordinate={container.coordinate}
      />
    );
  };

  return (
    <div className='row'>
      <div className="col-lg-11">
        <ContainerField property={parentContainerId} container={container}/>
      </div>
      {renderContainerDisplay()}
    </div>
  );
}

export default ContainerParentForm;
