import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { Link } from 'react-router-dom';
import { mapFormOptions } from './helpers.js';
import ContainerDisplay from './containerDisplay';


type ContainerProps = {
  current = object,
  data = object,
  editable = object,
  options = object,
  container = object,
  getParentContainerBarcodes = Function,
  getBarcodePathDisplay = Function,
  history = string,
  edit = Function,
  clearAll = Function,
  setCurrent = Function,
  setCheckoutList = Function,
  editContainer = Function,
  updateContainer = Function,
}

/**                                                                             
 * Container Component for displaying container information and actions.            
 *                                                                              
 * @component                                                                   
 * @param {ContainerProps} props - The properties passed to the component.         
 * @returns {ReactElement} React element representing the Container.
 */   
const BiobankContainer = ({
  current,
  data,
  editable,
  options,
  container,
  getParentContainerBarcodes,
  getBarcodePathDisplay,
  history,
  edit,
  clearAll,
  setCurrent,
  setCheckoutList,
  editContainer,
  updateContainer,
}) => {
  const drag = useCallback((e) => {
    const container = JSON.stringify(data.containers[e.target.id]);
    e.dataTransfer.setData('text/plain', container);
  }, [data.containers]);

  render() {
    const checkoutButton = () => {
      if (!(loris.userHasPermission('biobank_container_update')) ||
          (data.containers[container.id].childContainerIds.length == 0)) {
        return;
      }

      return (
        <div style = {{marginLeft: 'auto', height: '10%', marginRight: '10%'}}>
          <div
            className={!editable.containerCheckout && !editable.loadContainer ?
              'action-button update open' : 'action-button update closed'}
            title='Checkout Child Containers'
            onClick={() => edit('containerCheckout'};
          >
            <span className='glyphicon glyphicon-share'/>
          </div>
        </div>
      );
    };

    const parentBarcodes = getParentContainerBarcodes(container);
    const barcodes = mapFormOptions(data.containers, 'barcode');
    // delete values that are parents of the container
    Object.keys(parentBarcodes)
      .forEach((key) => Object.keys(barcodes)
        .forEach(
           (i) => (parentBarcodes[key] == barcodes[i])
                && delete barcodes[i]
        )
    );

    const barcodePathDisplay = getBarcodePathDisplay(parentBarcodes);
    const coordinates = data.containers[container.id].childContainerIds
      .reduce((result, id) => {
        const container = data.containers[id];
        if (container.coordinate) {
          result[container.coordinate] = id;
        }
        return result;
      }, {});

    const containerDisplay = (
      <div className='display-container'>
        {checkoutButton()}
        <ContainerDisplay
          history={history}
          data={data}
          container={container}
          barcodes={barcodes}
          current={current}
          options={options}
          dimensions={options.container.dimensions[container.dimensionId]}
          coordinates={coordinates}
          editable={editable}
          edit={edit}
          clearAll={clearAll}
          setCurrent={setCurrent}
          setCheckoutList={setCheckoutList}
          editContainer={editContainer}
          updateContainer={updateContainer}
        />
        <div style={{display: 'inline'}}>
          {barcodePathDisplay}
        </div>
      </div>
    );

    const containerList = () => {
      if (!container.childContainerIds) {
        return <div className='title'>This Container is Empty!</div>;
      }
      const childIds = container.childContainerIds;
      let listAssigned = [];
      let coordinateList = [];
      let listUnassigned = [];
      childIds.forEach((childId) => {
        if (!loris.userHasPermission('biobank_specimen_view')) {
          return;
        }

        const child = data.containers[childId];
        
        if (child.coordinate) {
          listAssigned.push(
            <div>
               <Link
                  key={childId}
                  to={`/barcode=${child.barcode}`}>
                    {child.barcode}
               </Link>
            </div>
          );
          const coordinate = getCoordinateLabel(child);
          coordinateList.push(<div>at {coordinate}</div>);
        } else {
          listUnassigned.push(
            <div>
            <Link
              key={childId}
              to={`/barcode=${child.barcode}`}
              id={child.id}
              draggable={true}
              onDragStart={this.drag}
            >
              {child.barcode}
            </Link>
            <br/>
            </div>
          );
        }
      });

      return (
        <div>
          <div className='title'>
            {listAssigned.length !== 0 ? 'Assigned Containers' : null}
          </div>
          <div className='container-coordinate'>
            <div>{listAssigned}</div>
            <div style={{paddingLeft: 10}}>{coordinateList}</div>
          </div>
            {listAssigned.length !==0 ? <br/> : null}
          <div className='title'>
            {listUnassigned.length !== 0 ? 'Unassigned Containers' : null}
          </div>
          {listUnassigned}
        </div>
      );
    };

    return (
      <div className="container-display">
        {containerDisplay}
        <div className='container-list'>
          {containerList()}
        </div>
      </div>
    );
  }
}

export default BiobankContainer;
