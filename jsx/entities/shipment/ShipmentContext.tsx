import { createContext, useContext, ReactNode } from 'react';
import { ShipmentHook } from './';

const ShipmentContext = createContext<ShipmentHook | null>(null);

interface ShipmentProviderProps {
  shipment: ShipmentHook;
  children?: ReactNode;
}

export const ShipmentProvider = ({                                                 
  children,                                                                        
  shipment                                                                         
}: ShipmentProviderProps): JSX.Element => {   
  return (
    <ShipmentContext.Provider value={shipment}>
      {children}
    </ShipmentContext.Provider>
  );
};

export const useShipmentContext = (): ShipmentHook => {
  const context = useContext(ShipmentContext);
  if (context === null) {
    throw new Error('useShipmentContext must be used within a ShipmentProvider');
  }
  return context;
};
