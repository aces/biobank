import { useCallback } from 'react';
import { IShipment, ILog, Shipment, Log } from './';
import { EntityHook, useEntity } from '../';


export interface ShipmentHook extends EntityHook<Shipment, IShipment> {                   
  addLog: (log: Partial<ILog>)  => void,                 
  setLog: (index: number, log: Partial<ILog>)  => void,                 
  removeLog: (index: number)  => void,                 
}  

export function useShipment(
  initialShipment: Partial<IShipment> = {}
): ShipmentHook {

  const shipment = useEntity<Shipment, IShipment>(() => new Shipment(initialShipment));
  const shipmentEntity = shipment.getEntity() as Shipment;

  const addLog = useCallback((log: Partial<ILog>) => {
    shipment.set('logs', shipmentEntity.addLog(log));
  }, [shipment]);

  const setLog = useCallback((index: number, log: Partial<ILog>) => {
    shipment.set('logs', shipmentEntity.setLog(index, log));
  }, [shipment]);

  const removeLog = useCallback((index: number) => {
    shipment.set('logs', shipmentEntity.removeLog(index));
  }, [shipment]);

  return {
    ...shipment,
    addLog,
    setLog,
    removeLog
  };
}

export interface LogHook extends EntityHook<Log, ILog> {                   
}  

export function useLog(
  initialLog: Partial<ILog> = {}
): LogHook {

  return useEntity<Log, ILog>(() => new Log(initialLog));
}
