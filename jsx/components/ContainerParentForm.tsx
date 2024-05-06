import { ReactElement, useState, useEffect } from 'react';
import { ContainerDisplay, ContainerField } from '../components';
import { ContainerHook } from '../entities';
import { useBiobankContext} from '../hooks';

const ContainerParentForm: React.FC<{
  container: ContainerHook,
  display?: boolean,
}> = ({
  container,
  display
}) => {
  const { options, containers } = useBiobankContext();

  // when a parent container is selected, the passed container gets updated
  useEffect(() => {
    const parentContainer = containers[container.parentContainer];
    if (parentContainer) {
      container.set('coordinate', null);
      container.set('temperature', parentContainer.temperature);
      container.set('center', parentContainer.center);
      container.set('status', parentContainer.status);
    }
  }, [container.parentContainer]);

  const renderContainerDisplay = () => {
    if (!(container.parentContainer && display)) {
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
        <ContainerField property={'parentContainer'}/>
      </div>
      {renderContainerDisplay()}
    </div>
  );
}

export default ContainerParentForm;
