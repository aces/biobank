import { ReactElement } from 'react';
import { ContainerDisplay } from '../components';
import { clone } from '../utils';
import { Data, Container, Options} from '../types';
import { ContainerHandler } from '../hooks';
import { SearchableDropdown } from 'Form';
import { ContainerAPI } from '../APIs';

type ContainerParentForm = {
  container: Container,
  contHandler: ContainerHandler,
  options: Options,
  display: boolean;
}

function ContainerParentForm({
  container,
  contHandler,
  options,
  display
}: ContainerParentForm): ReactElement {

  const [containers, setContainer] = useState(ContainerAPI.getAll());

  // TODO: there might be a better way to do this.
  const setInheritedProperties = (name, containerId) => {
    if (!containerId) {
      return;
    }

    const parentContainer = ContainerAPI.getById(containerId);
    contHandler.set('parentContainerId', parentContainer.id);
    contHandler.set('coordinate', null);
    contHandler.set('temperature', parentContainer.temperature);
    contHandler.set('centerId', parentContainer.centerId);
    contHandler.set('statusId', parentContainer.statusId);
  };

  // TODO: find a way to implement this again:
  // const removeChildContainers = (object, id) => {
  //   delete object[id];
  //   for (let key in containers) {
  //     if (id == containers[key].parentContainerId) {
  //       object = removeChildContainers(object, key);
  //     }
  //   }
  //   return object;
  // };

  
  let containerBarcodesNonPrimary = Object.values(containers)
  .reduce((result, container) => {
    const dimensions = options.container.dimensions[containers[
      container.id
    ].dimensionId];
    const capacity = dimensions.x * dimensions.y * dimensions.z;
    const available = capacity - container.childContainerIds.length;
    result[container.id] = container.barcode +
         ' (' +available + ' Available Spots)';
    return result;
  }, {});

  // TODO: find a way to implement this again:
  // Delete child containers from options if a container is being placed in a
  // another container.
  // if (container) {
  //   containerBarcodesNonPrimary = removeChildContainers(
  //     containerBarcodesNonPrimary,
  //     container.id
  //   );
  // }

  const renderContainerDisplay = () => {
    if (!(container.parentContainerId && display)) {
      return;
    }

    const coordinates = containers[
      container.parentContainerId
    ].childContainerIds
    .reduce((result, id) => {
      const container = containers[id];
      if (container.coordinate) {
          result[container.coordinate] = id;
      }
      return result;
    }, {});

    return (
      <ContainerDisplay
        container={container}
        containers={containers}
        dimensions={options.container.dimensions[containers[
          container.parentContainerId
        ].dimensionId]}
        coordinates={coordinates}
        parentContainerId={container.parentContainerId}
        options={options}
        select={true}
        selectedCoordinate={container.coordinate}
        setContainer={contHandler.set}
      />
    );
  };

  return (
    <div className='row'>
      <div className="col-lg-11">
        <SearchableDropdown
          name="parentContainerId"
          label="Parent Container Barcode"
          options={containerBarcodesNonPrimary}
          onUserInput={setInheritedProperties}
          value={container.parentContainerId}
        />
      </div>
      {renderContainerDisplay()}
    </div>
  );
}

export default ContainerParentForm;
