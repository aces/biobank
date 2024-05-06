import React, { createContext, ReactNode } from 'react';
import { ISpecimen, IContainer, IPool, IShipment } from '../entities';
import { Options } from '../types';
import {                                                                        
  OptionAPI,                                                                    
  SpecimenAPI,                                                                  
  ContainerAPI,                                                                 
  PoolAPI,                                                                      
  ShipmentAPI                                                                   
} from '../APIs'; 
import { useRequest, useStream } from '../hooks';

// TODO: bring context to another file:                                         
export interface BiobankContextType {                                              
  options?: Options,
  specimens?: Record<string, ISpecimen>,                                            
  containers?: Record<string, IContainer>,                                            
  pools?: Record<string, IPool>,                                            
  shipments?: Record<string, IShipment>,                                            
  contProg?: number,
  specProg?: number,
  poolProg?: number,
  shipProg?: number,
}                                                                               
                                                                                
const BiobankContext = React.createContext(undefined);

const optionsDef = {
  candidates: {},
  diagnoses: {},
  sessions: {},
  projects: {},
  centers: {},
  examiners: {},
  users: {},
  candidateSessions: {},
  sessionCenters: {},
  container: {
    types: {},
    typesPrimary: {},
    typesNonPrimary: {},
    dimensions: {},
    stati: {},
  },
  specimen: {
    types: {},
    typeUnits: {},
    typeContainerTypes: {},
    protocols: {},
    processes: {},
    protocolAttributes: {},
    processAttributes: {},
    units: {},
    attributes: {},
    attributeDatatypes: {},
  },
  shipment: {statuses: {}, types: {}}
};

export const BiobankProvider: React.FC = ({ children }) => {
  const { data: options, isLoading: optWait, error: optionsError }
    = useRequest('options', () => new OptionAPI().getAll(), optionsDef);
  
  const { data: containers, progress: contProg, error: containersError }
    = useStream(new ContainerAPI());
  
  const { data: specimens, progress: specProg, error: specimensError }
    = useStream(new SpecimenAPI());
  
  const { data: pools, progress: poolProg, error: poolsError }
    = useStream(new PoolAPI());
  
  const { data: shipments, progress: shipProg, error: shipmentsError }
    = useStream(new ShipmentAPI());

  // const update = useCallback((newShipments: any) => {
  //   setState(prevState => ({
  //     ...prevState,
  //     shipments: newShipments
  //   }));
  // }, []);

  const data = {
    options: options,
    containers: containers,
    specimens: specimens,
    shipments: shipments,
    pools: pools,
    contProg: contProg,
    specProg: specProg,
    poolProg: poolProg,
    shipProg: shipProg,
  }

  return (
    <BiobankContext.Provider value={data}>
      {children}
    </BiobankContext.Provider>
  );
};
                                                                                
export default BiobankContext;
