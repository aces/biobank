import React, { useState, useCallback, useEffect, useContext, ReactElement }  from 'react';
import { Link } from 'react-router-dom';
import { Data, Options, Specimen, Container, Dimension } from '../types';
import { clone } from '../utils';
import { useBiobankContext, useSpecimen, useContainer, useEditable, useBarcodePageContext} from '../hooks';
import { Globals, Header, ContainerDisplay } from '../components';
import { ContainerAPI } from '../APIs';
import { BarcodePageProvider } from '../contexts';
import Loader from 'jsx/Loader';
declare const loris: any;

function BarcodePathDisplay({
  container,
  contHandler
}) {
  const [barcodeData, setBarcodeData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const fetchData = async () => {
      try {
        setIsLoading(true);
        const parentBarcodes = await contHandler.getParentContainerBarcodes(container.barcode, []);
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
    return <Loader size={20}/>; // Replace with your actual loader component
  }

  return (
    <div>
      {barcodeData}
    </div>
  );
}

const initCurrent = {                                                           
  files: {},                                                                    
  list: {},                                                                     
  coordinate: null,                                                             
  sequential: false,                                                            
  count: null,                                                                  
  multiplier: 1,                                                                
};                                                                              

interface ContainerPageProps {
  history: any, // Define a more specific type if possible
  options: Options, // Define a more specific type if possible
  container: Container,
};

/**                                                                             
 * Render the barcode page
 *
 * @param {ContainerPageProps} props - The props for the component.             
 * @returns {ReactElement} The rendered component.                              
 */    
function ContainerPage({
  history,
  container: initContainer,
}: ContainerPageProps): ReactElement {

  const { editable, edit, clear } = useBarcodePageContext();
  const { options, data } = useBiobankContext();
  const { container, contHandler } = useContainer(initContainer);
  const [current, setCurrent] = useState(initCurrent);                          
  
  /**
   * Get the label for a coordinate in a container
   *
   * @param {Container}
   * @return {ReactElement | null}
   */
  function getCoordinateLabel(container: Container): ReactElement | null {
    const parentContainer = data.containers[container.parentContainerId];

    const formatCoordinate = (x: number, y: number, dimensions: Dimension): string => {
      const xVal = dimensions.xNum === 1 ? x : String.fromCharCode(64 + x);
      const yVal = dimensions.yNum === 1 ? y : String.fromCharCode(64 + y);
      return dimensions.xNum === 1 && dimensions.yNum === 1 
        ? `${x + (dimensions.x * (y - 1))}` 
        : `${yVal}${xVal}`;
    }

    if (parentContainer) {
      const dimensions = options.container.dimensions[parentContainer.dimensionId];
      let coordinateCount = 1;
  
      for (let y = 1; y <= dimensions.y; y++) {
        for (let x = 1; x <= dimensions.x; x++, coordinateCount++) {
          if (coordinateCount === container.coordinate) {
            return <span>{formatCoordinate(x, y, dimensions)}</span>;
          }
        }
      }
    }
  
    return null;
  }

  const drag = useCallback((e) => {                                             
    const container = JSON.stringify(data.containers[e.target.id]);             
    e.dataTransfer.setData('text/plain', container);                            
  }, [data.containers]);                                                        
                                                                                
                                                                                
  const updateCurrent = (name: string, value: any) =>  {                        
    setCurrent(prevCurr => ({                                                   
      ...prevCurr,                                                              
      [name]: value                                                             
    }));                                                                        
  }                                                                             
                                                                                
  function setCheckoutList(container: Container): void {                        
    // Clear current container field.                                           
    updateCurrent('containerId', 1)                                             
    updateCurrent('containerId', null);                                         
    const list = clone(current.list);                                           
    list[container.coordinate] = container;                                     
    updateCurrent('list', list);                                                
  }                                                                             
                                                                                
  const checkoutButton = () => {                                                
    if (                                                                        
      !loris.userHasPermission('biobank_container_update') ||                   
      data.containers[container.id].childContainerIds.length == 0               
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
                                                                                
  const barcodes = mapFormOptions(data.containers, 'barcode');                  
  // delete values that are parents of the container                            
                                                                                
  //TODO re-instate this in some form!                                          
  //  Object.keys(parentBarcodes).forEach((key) =>                              
  //    Object.keys(barcodes).forEach(                                          
  //      (i) =>                                                                
  //        parentBarcodes[key] == barcodes[i] && delete barcodes[i]            
  //    )                                                                       
  //  );                                                                        
                                                                                
  const coordinates = data.containers[container.id].childContainerIds.reduce(   
    (result, id) => {                                                           
      const container = data.containers[id];                                    
      if (container.coordinate) {                                               
        result[container.coordinate] = id;                                      
      }                                                                         
      return result;                                                            
    },                                                                          
    {}                                                                          
  );                                                                            
                                                                                
  const containerDisplay = (                                                    
    <div className='display-container'>                                         
      {checkoutButton()}                                                        
      <ContainerDisplay                                                         
        history={history}                                                       
        container={container}                                                   
        barcodes={barcodes}                                                     
        current={current}                                                       
        options={options}                                                       
        dimensions={options.container.dimensions[container.dimensionId]}        
        coordinates={coordinates}                                               
        editable={editable}                                                     
        edit={edit}                                                             
        clearAll={clear}                                                     
        setCurrent={setCurrent}                                                 
        setCheckoutList={setCheckoutList}                                       
      />                                                                        
      <div style={{ display: 'inline' }}>
        <BarcodePathDisplay
          container={container}
          contHandler={contHandler}
        />
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
        return; // This is the correct place for the return statement           
      }                                                                         
                                                                                
      const child = data.containers[childId];                                   
                                                                                
      if (child.coordinate) {                                                   
        listAssigned.push(                                                      
          <div key={childId}>                                                   
            <Link to={`/barcode=${child.barcode}`}>                             
              {child.barcode}                                                   
            </Link>                                                             
          </div>                                                                
        );                                                                      
        const coordinate = getCoordinateLabel(child);                           
        coordinateList.push(<div key={coordinate}>at {coordinate}</div>);       
      } else {                                                                  
        listUnassigned.push(                                                    
          <div key={childId}>                                                   
            <Link                                                               
              to={'/barcode='+child.barcode}                                    
              id={child.id}                                                     
              draggable={true}                                                  
              onDragStart={drag}                                                
            >                                                                   
              {child.barcode}                                                   
            </Link>                                                             
            <br />                                                              
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
      <BarcodePageProvider>
        <Link to={`/`}>
          <span className='glyphicon glyphicon-chevron-left'/>
          Return to Filter
        </Link>
        <Header
          options={options}
          clearAll={clear}
          render={() => <BarcodePathDisplay container={container}/>}
        />
        <div className='summary'>
          <Globals
            data={data}
            options={options}
            container={container}
            contHandler={contHandler}
            clearAll={clear}
            getCoordinateLabel={getCoordinateLabel}
          />
          <div className='container-display'>                                         
            {containerDisplay}                                                        
            <div className='container-list'>{containerList()}</div>                   
          </div>                                                                      
        </div>
      </BarcodePageProvider>
  );
}

export default ContainerPage;
