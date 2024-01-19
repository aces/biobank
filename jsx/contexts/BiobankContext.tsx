import React, { createContext, ReactNode } from 'react';
import { Options, Specimen, Container, Pool, Shipment } from '../types';

// TODO: bring context to another file:                                         
interface BiobankContextType {                                              
  options?: Options,
  specimens?: Record<string, Specimen>,                                            
  containers?: Record<string, Container>,                                            
  pools?: Record<string, Pool>,                                            
  shipments?: Record<string, Shipment>,                                            
}                                                                               
                                                                                
const BiobankContext = React.createContext<BiobankContextType>({});

interface BiobankProviderProps {
  children: ReactNode;
  biobankData: BiobankContextType
}

export const BiobankProvider: React.FC<BiobankProviderProps> = ({
  children,
  biobankData,
}) => {

  return (
    <BiobankContext.Provider value={{ editable, edit, clear }}>
      {children}
    </BiobankContext.Provider>
  );
};
                                                                                
export default BiobankContext;
