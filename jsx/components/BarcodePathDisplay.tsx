import { useState, useEffect } from 'react';
import { IContainer, Container } from '../entities';
import Loader from 'jsx/Loader';
import { Link } from 'react-router-dom';

export const BarcodePathDisplay = (
  { container }: { container: Partial<IContainer> }
) => {
  const [barcodeData, setBarcodeData] = useState(null);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    const fetchData = async () => {
      try {
        setIsLoading(true);
        const containerEntity = new Container(container);
        const parentBarcodes = await
        containerEntity.getParentContainerBarcodes(container.barcode, []);
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
  }, [container]); //XXX: currently this is re-rendering even if the parent doesn't change.

  if (isLoading) {
    return <Loader size={20}/>; // Replace with your actual loader component
  }

  return <>{barcodeData}</>;
}
