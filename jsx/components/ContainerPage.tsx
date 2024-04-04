import React, { useState, useCallback, useEffect, useContext, ReactElement }  from 'react';
import { Link } from 'react-router-dom';
import { Options, Specimen, Container, Dimension } from '../types';
import { clone, mapFormOptions } from '../utils';
import {
  useBiobankContext,
  useSpecimen,
  useContainer,
  useContainerById,
  useEditable,
  useBarcodePageContext
} from '../hooks';
import { Globals, Header, ContainerDisplay } from '../components';
import { ContainerAPI } from '../APIs';
import { BarcodePageProvider } from '../contexts';
import Loader from 'jsx/Loader';
declare const loris: any;

function BarcodePathDisplay({
  container,
}): ReactElement {
  const [barcodeData, setBarcodeData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const fetchData = async () => {
      try {
        setIsLoading(true);
        const parentBarcodes = await container.getParentContainerBarcodes(container.barcode, []);
        const displayData = parentBarcodes.map((barcode, index) => {
  //     const container = Object.values(data.containers)
  //       .find(
  //         (container) =>
  //           container.barcode == parentBarcodes[index+1]
  //       );
  //     let coordinateDisplay;
  //     if (container) {
  //       const coordinate = getCoordinateLabel(container);
  //       coordinateDisplay = <b>{'-'+(coordinate || 'UAS')}</b>;
  //     }
          return (
            <span className='barcodePath'>
              {index !== 0 && ': '}
              <Link key={index} to={'/containers/'+barcode}>{barcode}</Link>
             {/*coordinateDisplay*/}
            </span>
          );
        });
        setBarcodeData(displayData);
      } catch (error) {
        console.error("Error fetching barcode data:", error);
        // Handle error appropriately
      } finally {
        setIsLoading(false);
      }
    };

    fetchData();
  }, [container.parentContainerBarcode]); //XXX: currently this is re-rendering even if the parent doesn't change.

  if (isLoading) {
    return <Loader size={20}/>;
  }

  return (
    <div>
      {barcodeData}
    </div>
  );
}

const initCurrent = {                                                           
  list: {},                                                                     
};                                                                              

export type Current = {
  list?: Record<string, any>,
  prevCoordinate?: number,
  coordinate?: number,
  barcode?: string,
  sequential?: boolean,
  containerId?: string
}

interface ContainerPageProps {
  barcode: string,
};

/**                                                                             
 * Render the barcode page
 *
 * @param {ContainerPageProps} props - The props for the component.             
 * @returns {ReactElement} The rendered component.                              
 */    
export default function ContainerPage({
  barcode
}: ContainerPageProps): ReactElement {

  const [current, setCurrent] = useState<Current>(initCurrent);                          
  const { editable, edit, clear } = useBarcodePageContext();
  const { options, containers } = useBiobankContext();
  const container = useContainer(containers[barcode]);

  /**
   * Get the label for a coordinate in a container
   *
   * @param {Container}
   * @return {ReactElement | null}
   */
  function getCoordinateLabel(container: Container): ReactElement | null {
    const parentContainer = containers[container.parentContainerBarcode];
    if (!parentContainer) return null;

    const dimensions = options.container.dimensions[parentContainer.dimensionId];
    if (!dimensions) return null;

    const formatCoordinate = (x: number, y: number, dimensions: Dimension): string => {
      const xVal = dimensions.xNum === 1 ? x : String.fromCharCode(64 + x);
      const yVal = dimensions.yNum === 1 ? y : String.fromCharCode(64 + y);
      return dimensions.xNum === 1 && dimensions.yNum === 1 
        ? `${x + (dimensions.x * (y - 1))}` 
        : `${yVal}${xVal}`;
    }

    let coordinateCount = 1;
    for (let y = 1; y <= dimensions.y; y++) {
      for (let x = 1; x <= dimensions.x; x++, coordinateCount++) {
        if (coordinateCount === container.coordinate) {
          return <span>{formatCoordinate(x, y, dimensions)}</span>;
        }
      }
    }

    return null;
  }

  const drag = useCallback((e) => {                                             
    const cont = JSON.stringify(containers[e.target.id]);             
    e.dataTransfer.setData('text/plain', cont);                            
  }, [containers]);                                                        
                                                                                
  const updateCurrent = (name: string, value: any) =>  {                        
    setCurrent(prevCurr => ({                                                   
      ...prevCurr,                                                              
      [name]: value                                                             
    }));                                                                        
  }                                                                             

  function setCheckoutList(selectedContainer: Container): void {                        
    // Clear current container field.                                           
    updateCurrent('containerId', 1)                                             
    updateCurrent('containerId', null);                                         
    const list = clone(current.list);                                           
    list[container.coordinate] = selectedContainer;                                     
    updateCurrent('list', list);                                                
  }                                                                             

  const checkoutButton = () => {                                                
    if (                                                                        
      !loris.userHasPermission('biobank_container_update') ||                   
      containers[container.barcode].childContainerBarcodes.length == 0               
    ) {                                                                         
      return;                                                                   
    }                                                                           
                                                                                
    return (                                                                    
      <div style={{ marginLeft: 'auto', height: '10%', marginRight: '10%' }}>   
        <div                                                                    
          className={                                                           
            !editable.containerCheckout && !editable.loadContainer              
              ? 'action-button update open'                                     
              : 'action-button update closed'                                   
          }                                                                     
          title='Checkout Child Containers'                                     
          onClick={() => edit('containerCheckout')}                             
        >                                                                       
          <span className='glyphicon glyphicon-share' />                        
        </div>                                                                  
      </div>                                                                    
    );                                                                          
  };                                                                            

  const barcodes = mapFormOptions(containers, 'barcode');                  

  // XXX: KEEP COMMENTED OUT
  // delete values that are parents of the container                            
                                                                                
  //TODO re-instate this in some form!                                          
  //  Object.keys(parentBarcodes).forEach((key) =>                              
  //    Object.keys(barcodes).forEach(                                          
  //      (i) =>                                                                
  //        parentBarcodes[key] == barcodes[i] && delete barcodes[i]            
  //    )                                                                       
  //  );                                                                        
  // UP UNTIL HERE


  const containerDisplay = (                                                    
    <div className='display-container'>                                         
      {checkoutButton()}                                                        
      <ContainerDisplay                                                         
        container={container}                                                   
        barcodes={barcodes}                                                     
        current={current}                                                       
        dimensions={options.container.dimensions[container.dimensionId]}        
        editable={editable}                                                     
        edit={edit}                                                             
        clearAll={clear}                                                     
        updateCurrent={updateCurrent}                                                 
        setCheckoutList={setCheckoutList}                                       
      />                                                                        
      <div style={{ display: 'inline' }}>
        <BarcodePathDisplay
          container={container}
        />
      </div>             
    </div>                                                                      
  );                                                                            

  const containerList = () => {                                                 
    if (!container.childContainerBarcodes) {                                         
      return <div className='title'>This Container is Empty!</div>;             
    }                                                                           
                                                                                
    let listAssigned = [];                                                      
    let coordinateList = [];                                                    
    let listUnassigned = [];                                                    
                                                                                
    container.childContainerBarcodes.forEach(([coordinate, barcode]) => {                                          
      if (!loris.userHasPermission('biobank_specimen_view')) {                  
        return; // This is the correct place for the return statement           
      }                                                                         

      // Check if the current entry is for 'unassigned' containers
      if (coordinate === 'unassigned') {
        container.childContainerBarcodes.unassigned.forEach(barcode => {
          listUnassigned.push(
            <div key={barcode}>
              <Link
                to={'/barcode=' + barcode}
                id={barcode}
                draggable={true}
                onDragStart={drag}
              >
                {barcode}
              </Link>
              <br />
            </div>
          );
        });
      } else {
        // Process assigned containers
        listAssigned.push(
          <div key={barcode}>
            <Link to={`/barcode=${barcode}`}>
              {barcode}
            </Link>
          </div>
        );
        const coordinateLabel = getCoordinateLabel(containers.barcodes);
        coordinateList.push(<div key={barcode}>at {coordinateLabel}</div>);
      }
    });
                                                                                
    return (                                                                    
      <div>                                                                     
        <div className='title'>                                                 
          {listAssigned.length !== 0 ? 'Assigned Containers' : null}            
        </div>                                                                  
        <div className='container-coordinate'>                                  
          <div>{listAssigned}</div>                                             
          <div style={{ paddingLeft: 10 }}>{coordinateList}</div>               
        </div>                                                                  
        {listAssigned.length !== 0 ? <br /> : null}                             
        <div className='title'>                                                 
          {listUnassigned.length !== 0 ? 'Unassigned Containers' : null}        
        </div>                                                                  
        {listUnassigned}                                                        
      </div>                                                                    
    );                                                                          
  };                                                                            

  return (
    <>
      <Link to={`/`}>
        <span className='glyphicon glyphicon-chevron-left'/>
        Return to Filter
      </Link>
      <Header
        container={container}
        clearAll={clear}
        render={() => <BarcodePathDisplay container={container}/>}
      />
      <div className='summary'>
        <Globals
          container={container}
          clearAll={clear}
          getCoordinateLabel={getCoordinateLabel}
        />
        <div className='container-display'>                                         
          {containerDisplay}                                                        
          <div className='container-list'>{containerList()}</div>                   
        </div>                                                                      
      </div>
    </>
  );
}
