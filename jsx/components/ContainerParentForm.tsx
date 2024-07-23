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
    const parent = containers[container.parent.barcode];
    if (parent) {
      container.set('coordinate', null);
      container.set('temperature', parent.temperature);
      container.set('center', parent.center);
      container.set('status', parent.status);
    }
  }, [container.parent]);

  const renderContainerDisplay = () => {
    if (!(container.parent && display)) {
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
        <ContainerField property={'parent'}/>
      </div>
      {renderContainerDisplay()}
    </div>
  );
}

export default ContainerParentForm;
