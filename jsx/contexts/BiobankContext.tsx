import React, { createContext, ReactNode } from 'react';
import { Options, Specimen, Container, Pool, Shipment } from '../types';

// TODO: bring context to another file:                                         
export interface BiobankContextType {                                              
  options?: Options,
  specimens?: Record<string, Specimen>,                                            
  containers?: Record<string, Container>,                                            
  pools?: Record<string, Pool>,                                            
  shipments?: Record<string, Shipment>,                                            
}                                                                               
                                                                                
const BiobankContext = React.createContext<BiobankContextType>({});

export const BiobankProvider: React.FC<{
  context: BiobankContextType,
}> = ({
  children,
  context,
}) => {
  return (
    <BiobankContext.Provider value={context}>
      {children}
    </BiobankContext.Provider>
  );
};
                                                                                
export default BiobankContext;
