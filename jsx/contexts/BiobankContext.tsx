import React, { createContext, useCallback, ReactNode } from 'react';
import { ISpecimen, IContainer, IPool, IShipment } from '../entities';
import {                                                                        
  SpecimenAPI,                                                                  
  ContainerAPI,                                                                 
  PoolAPI,                                                                      
  ShipmentAPI                                                                   
} from '../APIs'; 
import { useRequest, useStream, Stream } from '../hooks';

export enum EntityType {
  Containers = 'containers',
  Specimens = 'specimens',
  Pools = 'pools',
  Shipments = 'shipments',
}

export interface BiobankContextType {
  containers: Stream<IContainer>;
  specimens: Stream<ISpecimen>;
  pools: Stream<IPool>;
  shipments: Stream<IShipment>;
  updateEntity: (type: EntityType, id: string, newEntity: any) => void;
  updateEntities: (type: EntityType, newEntities: Record<string, any>) => void;
  initializeEntity: (type: EntityType) => void;
}

const BiobankContext = React.createContext<BiobankContextType | null>(null);

export const BiobankProvider = ({
  children
}: { children: ReactNode } ) => {
  const streams = {
    [EntityType.Containers]: useStream(new ContainerAPI()),
    [EntityType.Specimens]: useStream(new SpecimenAPI()),
    [EntityType.Pools]: useStream(new PoolAPI()),
    [EntityType.Shipments]: useStream(new ShipmentAPI()),
  };

  const updateEntity = useCallback((type: EntityType, id: string, newEntity: any) => {
    const stream = streams[type];
    if (stream && stream.data) {
      stream.data = { ...stream.data, [id]: newEntity };
    }
  }, [streams]);

  const updateEntities = useCallback((type: EntityType, newEntities: Record<string, any>) => {
    const stream = streams[type];
    if (stream) {
      stream.data = newEntities;
    }
  }, [streams]);

  const initializeEntity = useCallback((type: EntityType) => {
    const stream = streams[type];
    if (stream && !stream.initialized) {
      stream.initialize();
    }
  }, [streams]);

  return (
    <BiobankContext.Provider
      value={{ ...streams, updateEntity, updateEntities, initializeEntity }}
    >
      {children}
    </BiobankContext.Provider>
  );
};
                                                                                
export default BiobankContext;
