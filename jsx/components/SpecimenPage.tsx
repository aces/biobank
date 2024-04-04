import React, { useState, useEffect, ReactElement } from 'react';
import { Link } from 'react-router-dom';
import { Options, Specimen, ProcessType, Container, Dimension } from '../types';
import { clone, isEmpty } from '../utils';
import { useSpecimen, useContainer, useBarcodePageContext, useBiobankContext } from '../hooks';
import { Globals, Header, ProcessPanel } from '../components';
import { BarcodePageProvider } from '../contexts';
import LoadingBar from 'jsx/LoadingBar';
import Loader from 'jsx/Loader';

function BarcodePathDisplay({
  container,
}) {
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
    return <Loader size={20}/>; // Replace with your actual loader component
  }

  return (
    <div>
      {barcodeData}
    </div>
  );
}

type SpecimenPageProps = {
  specimen: Specimen,
};

/**                                                                             
 * Render the barcode page
 *
 * @param {SpecimenPageProps} props - The props for the component.             
 * @returns {ReactElement} The rendered component.                              
 */    
function SpecimenPage({
  specimen: initSpecimen,
}: SpecimenPageProps): ReactElement {

  const [loading, setLoading] = useState(false);
  const specimen = useSpecimen(initSpecimen);
  const container = useContainer(initSpecimen.container);
  const { clear } = useBarcodePageContext();
  const { options, containers } = useBiobankContext();

  function clearAll() {
    setLoading(false);
    contHandler.reset();
    specHandler.reset();
    clear();
  }

  /**
   * Get the label for a coordinate in a container
   *
   * @param {Container}
   * @return {ReactElement | null}
   */
  function getCoordinateLabel(container: Container): ReactElement | null {
    const parentContainer = containers[container.parentContainerId];

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

  return (
    <BarcodePageProvider>
      <Link to={`/`}>
        <span className='glyphicon glyphicon-chevron-left'/>
        Return to Filter
      </Link>
      <Header
        specimen={specimen}
        container={container}
        clearAll={clearAll}
        render={() => <BarcodePathDisplay container={container}/>}
      />
      <div className='summary'>
        <Globals
          specimen={specimen}
          container={container}
          clearAll={clearAll}
          getCoordinateLabel={getCoordinateLabel}
        />
        <div className="processing">
          <ProcessPanel
            process={ProcessType.Collection}
            specimen={specimen}
            clearAll={clearAll}
          />
          <ProcessPanel
            process={ProcessType.Preparation}
            specimen={specimen}
            clearAll={clearAll}
          />
          <ProcessPanel
            process={ProcessType.Analysis}
            specimen={specimen}
            clearAll={clearAll}
          />
        </div>
      </div>
    </BarcodePageProvider>
  );
}

export default SpecimenPage;
