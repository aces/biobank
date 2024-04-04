import { useEffect, ReactElement } from 'react';
import { useNavigate } from 'react-router-dom';
import Swal from 'sweetalert2';
import { useBiobankContext } from '../hooks';
import { mapFormOptions } from '../utils';
import { Container, Specimen, Dimension } from '../types';
import { ContainerAPI } from '../APIs';
import { Current } from './ContainerPage';
import Form from 'Form';
const {
  FormElement,
  StaticElement,
  SearchableDropdown,
  TextboxElement,
  CheckboxElement,
  ButtonElement,
} = Form;
declare const loris: any;

interface ContainerDisplayProps {
  barcodes?: Record<string, string>; // Assuming barcodes is a mapping of ids to barcode strings
  current?: Current,
  editable?: {
    loadContainer?: boolean;
    containerCheckout?: boolean;
  };
  select?: boolean;
  container: Container,
  selectedCoordinate?: number;
  clearAll?: () => void;
  updateCurrent?: (name: string, value: any) => void,
  setCheckoutList?: (container: any) => void; // Specify a more precise type for container if possible
  edit?: (action: string) => Promise<void>; // Adjust the type of action based on actual usage
}

function ContainerDisplay({
    barcodes,
    current,
    editable,
    select = false,
    container, // Further specify this type based on the actual object structure
    selectedCoordinate,
    clearAll,
    updateCurrent,
    setCheckoutList,
    edit,
}: ContainerDisplayProps): ReactElement {

  const { options, containers, specimens } = useBiobankContext();
  const navigate = useNavigate();
  const dimensions =
    options.container.dimensions[containers[container.parentContainerId].dimensionId];

  useEffect(() => {
    // TODO: Use react-based approach isntead of jquery
    // $('[data-toggle="tooltip"]').tooltip();
  });

  const redirectURL = (e: React.MouseEvent<HTMLDivElement>) => {
    const coordinate = (e.target as HTMLElement).id;
    if (container.childContainerBarcodes[coordinate]) {
      const barcode = containers[container.childContainerBarcodes[coordinate]].barcode;
      navigate(`/barcode=${barcode}`);
    }
  };

  const allowDrop = (e: React.MouseEvent<HTMLDivElement>) => e.preventDefault();

  const drag = (e: React.DragEvent<HTMLDivElement>) => {
    // $('[data-toggle="tooltip"]').tooltip('hide');
    // TODO: TO BE FIXED 
    // const container = JSON.stringify(containers[coordinates[(e.target as
    //                                                          HTMLElement).id]]);
    // e.dataTransfer.setData('text/plain', container);
  };

  const drop = (e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault();
    const container = JSON.parse(e.dataTransfer.getData('text/plain'));
    const newCoordinate = parseInt(e.currentTarget.id);
    container.coordinate = newCoordinate;
    new ContainerAPI().update(container);
  };

  const increaseCoordinate = (coordinate: number): void => {
    const capacity = dimensions.x * dimensions.y * dimensions.z;
    let nextCoordinate = coordinate +1 ;
    while (Object.keys(container.childContainerBarcodes).includes(nextCoordinate.toString()) && nextCoordinate <= capacity) {
      nextCoordinate++;
    }

    if (nextCoordinate > capacity) {
      clearAll();
    } else {
      updateCurrent('coordinate', nextCoordinate);
    }
  };

  const setBarcode = (name: string, barcode: string): void => {
    updateCurrent('barcode', barcode);
  };

  const loadContainer = (): void => {
    const barcode = current.barcode;
    const containerId = Object.keys(barcodes)
      .find((id) => barcodes[id] === barcode);

    if (!containerId) {
      return;
    }

    const newContainer = {
      ...containers[containerId],
      parentContainerId: container.id,
      coordinate: current.coordinate,
    }

    new ContainerAPI().update(newContainer)
    .then(() => {
      if (current.sequential) {
        increaseCoordinate(current.coordinate);
        updateCurrent('barcode', null);
      } else {
        clearAll();
      }
    });

    updateCurrent('prevCoordinate', newContainer.coordinate);
  };

  const checkoutContainers = (): void => {
    const checkoutPromises = Object.values(current.list).map((container:
                                                              Container) => {
      const updatedContainer = { ...container, parentContainerId: null, coordinate: null };
      return new ContainerAPI().update(updatedContainer);
    });

    Promise.all(checkoutPromises)
    .then(() => clearAll())
    .then(() => Swal.fire('Containers Successfully Checked Out!', '', 'success'))
    .catch(error => {
      // Handle any errors that occur during the update
      console.error("Error checking out containers:", error);
      Swal.fire('Error', 'There was a problem checking out the containers.', 'error');
    });

  };

  const barcodeField = (editable||{}).loadContainer && 
    <TextboxElement
      name='barcode'
      label='Barcode'
      onUserInput={setBarcode}
      value={current.barcode}
      placeHolder='Please Scan or Type Barcode'
      autoFocus={true}
    />

  let load = (
    <div className={((editable||{}).loadContainer) ? 'open' : 'closed'}>
      <FormElement>
        <StaticElement
          label='Note'
          text='Scan Containers to be Loaded. If Sequential is checked,
           the Coordinate will Auto-Increment after each Load.'
        />
        <CheckboxElement
          name='sequential'
          label='Sequential'
          value={current.sequential}
          onUserInput={updateCurrent}
        />
        {barcodeField}
        <ButtonElement
          label='Load'
          onUserInput={loadContainer}
        />
        <StaticElement
          text={<a onClick={clearAll} style={{cursor: 'pointer'}}>Cancel</a>}
        />
      </FormElement>
    </div>
  );

  // // place container children in an object
  // let children = {};
  // if ((container||{}).childContainerBarcodes) {
  //   Object.values(containers).map((c) => {
  //     container.childContainerIds.forEach((id) => {
  //       if (c.id == id) {
  //         children[id] = c;
  //       }
  //     });
  //   });
  // }

  // if ((editable||{}).containerCheckout) {
  //   // Only children of the current container can be checked out.
  //   let barcodes = mapFormOptions(children, 'barcode');

  //   barcodeField = (
  //     <SearchableDropdown
  //       name='barcode'
  //       label='Barcode'
  //       options={barcodes}
  //       onUserInput={(name, value) => {
  //         value && setCheckoutList(children[value]);
  //       }}
  //       value={current.containerId}
  //       placeHolder='Please Scan or Select Barcode'
  //       autoFocus={true}
  //     />
  //   );
  // }

  let checkout = (
    <div className={((editable||{}).containerCheckout) ? 'open' : 'closed'}>
      <FormElement>
        <StaticElement
          label='Note'
          text="Click, Select or Scan Containers to be
                Unloaded and Press 'Confirm'"
        />
        {barcodeField}
        <ButtonElement
          label='Confirm'
          type='button'
          onUserInput={checkoutContainers}
        />
        <StaticElement
          text={<a onClick={clearAll} style={{cursor: 'pointer'}}>Cancel</a>}
        />
      </FormElement>
    </div>

  );

  // TODO: This will eventually need to be reworked and cleaned up
  let display;
  let column = [];
  let row = [];
  let coordinate = 1;
  if (dimensions) {
    for (let y=1; y <= dimensions.y; y++) {
      column = [];
      for (let x=1; x <= dimensions.x; x++) {
        let nodeWidth = (500/dimensions.x) - (500/dimensions.x * 0.08);
        let nodeStyle = {width: nodeWidth};
        let nodeClass = 'node';
        let tooltipTitle = null;
        let title = null;
        let dataHtml = false;
        let dataToggle = null;
        let dataPlacement = null;
        let draggable = false;
        let onDragStart = null;
        let onDragOver = allowDrop;
        let onDrop = drop;
        let onClick = null;
        const optcon = options.container;

        if (!select) {
          if ((container.childContainerBarcodes[coordinate])) {
            onClick = redirectURL;
            if (coordinate in current.list) {
              nodeClass = 'node checkout';
            } else if (coordinate == current.prevCoordinate) {
              nodeClass = 'node new';
            } else {
              nodeClass = 'node occupied';
            }

            dataHtml = true;
            dataToggle = 'tooltip';
            dataPlacement = 'top';
            // This is to avoid a console error
            const childContainer = containers[container.childContainerBarcodes[coordinate]];
            tooltipTitle =
              '<h5>'+childContainer.barcode+'</h5>' +
              '<h5>'+optcon.types[childContainer.typeId].label+'</h5>' +
              '<h5>'+optcon.stati[childContainer.statusId].label+'</h5>';
            
            draggable = !loris.userHasPermission(
               'biobank_container_update') ||
                        editable.loadContainer ||
                        editable.containerCheckout
                        ? false : true;
            onDragStart = drag;

            if (editable.containerCheckout) {
              onClick = (e) => {
                const checkoutContainer = containers[container.childContainerBarcodes[e.target.id]];
                setCheckoutList(checkoutContainer);
              };
            }
            if (editable.loadContainer) {
              onClick = null;
            }
            onDragOver = null;
            onDrop = null;
          } else if (loris.userHasPermission('biobank_container_update') &&
                     !editable.containerCheckout) {
            nodeClass = coordinate == current.coordinate ?
              'node selected' : 'node load';
            title = 'Load...';
            onClick = (e) => {
              let containerId = e.target.id;
              edit('loadContainer')
              .then(() => updateCurrent('coordinate', containerId));
            };
          }
        }

        if (select) {
          if (coordinate == selectedCoordinate) {
            nodeClass = 'node occupied';
          } else if (!container.childContainerBarcodes) {
            nodeClass = 'node available';
            onClick = (e) => container.set('coordinate', e.target.id);
          } else if (container.childContainerBarcodes) {
            if (!container.childContainerBarcodes[coordinate]) {
              nodeClass = 'node available';
              onClick = (e) => container.set('coordinate', e.target.id);
            } else if (container.childContainerBarcodes[coordinate]) {
              const childContainer = containers[container.childContainerBarcodes[coordinate]];
              const specimen = Object.values(specimens as Record<string, Specimen>)
                .find((specimen: Specimen) => specimen.containerId == childContainer.id);
              let quantity = '';
              if (specimen) {
                quantity = `<h5>${specimen.quantity +
                    ' '+options.specimen.units[specimen.unitId].label}</h5>`;
              }
              dataHtml = true;
              dataToggle = 'tooltip';
              dataPlacement = 'top';
              tooltipTitle =
                `<h5>${childContainer.barcode}</h5>` +
                `<h5>${optcon.types[childContainer.typeId].label}</h5>` +
                quantity +
                `<h5>${optcon.stati[childContainer.statusId].label}</h5>`;
            }
          }
        }

        let coordinateDisplay;
        if (dimensions.xNum == 1 && dimensions.yNum == 1) {
          coordinateDisplay = x + (dimensions.x * (y-1));
        } else {
          const xVal = dimensions.xNum == 1 ? x : String.fromCharCode(64+x);
          const yVal = dimensions.yNum == 1 ? y : String.fromCharCode(64+y);
          coordinateDisplay = yVal+''+xVal;
        }

        column.push(
          <div
            key={x}
            id={coordinate.toString()}
            title={title}
            className={nodeClass}
            data-html={dataHtml}
            data-toggle={dataToggle}
            data-placement={dataPlacement}
            data-original-title={tooltipTitle}
            style={nodeStyle}
            onClick={onClick}
            draggable={draggable}
            onDragStart={onDragStart}
            onDragOver={onDragOver}
            onDrop={onDrop}
          >
            {coordinateDisplay}
          </div>
        );

        coordinate++;
      }

      let rowHeight = (500/dimensions.y) - (500/dimensions.y * 0.08);
      // let rowMargin = (500/dimensions.y * 0.04);
      let rowStyle = {height: rowHeight};

      row.push(<div key={y} className='row' style={rowStyle}>{column}</div>);
    }

    display = row;
  }

  return (
    <>
      <div style={{width: 500}}>
        {checkout}
        {load}
      </div>
      <div className='display'>
        {display}
      </div>
    </>
  );
}

export default ContainerDisplay;
