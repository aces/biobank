import { useBiobankContext } from '../hooks';
import { IContainer, Dimension } from '../entities';

export const CoordinateLabel: React.FC<{
  container: Partial<IContainer>
}> = ({
  container
}) => {   
  const { options, containers } = useBiobankContext();

  const parentContainer = containers[container.parentContainer];       
  if (!parentContainer) return null;                                          
                                                                              
  const dimension = container.dimension;
  if (!dimension) return null;                                               
                                                                              
  const formatCoordinate = (x: number, y: number, dimension: Dimension): string => {
    const xVal = dimension.xNum === true ? x : String.fromCharCode(64 + x);     
    const yVal = dimension.yNum === true ? y : String.fromCharCode(64 + y);     
    return dimension.xNum === true && dimension.yNum === true                    
      ? `${x + (dimension.x * (y - 1))}`                                     
      : `${yVal}${xVal}`;                                                     
  }                                                                           
                                                                              
  let coordinateCount = 1;                                                    
  for (let y = 1; y <= dimension.y; y++) {                                   
    for (let x = 1; x <= dimension.x; x++, coordinateCount++) {              
      if (coordinateCount === container.coordinate) {                         
        return <span>{formatCoordinate(x, y, dimension)}</span>;             
      }                                                                       
    }                                                                         
  }                                                                           
                                                                              
  return null;                                                                
}   
