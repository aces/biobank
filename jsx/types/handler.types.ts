import { Specimen, Container, Process, Shipment, Log} from '../types';

export interface GenericStateHandler<T extends object> {                               
  set: (name: keyof T, value: any) => void,                                     
  remove: (name: keyof T) => void,                                              
  reset: () => void,                                                            
  validate: () => Record<string, string>,                                       
  errors: Record<string, string>,                                               
}   

export interface SpecimenHandler extends GenericStateHandler<Specimen> {        
   setProcess: (process: 'collection' | 'preparation' | 'analysis', name: keyof Process, value: any) => void,
   setData: (process: 'collection' | 'preparation' | 'analysis', key: string, value: any) => void,
}   

export interface ContainerHandler extends GenericStateHandler<Container> {      
  getParentContainerBarcodes: (barcode: string, barcodes: string[]) => Promise<string[]>,
}   

export interface ShipmentHandler extends GenericStateHandler<Shipment> {      
  setLogs: (name: keyof Log, value: any, index: number) => void,
}   
