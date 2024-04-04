import { Specimen, Container, Pool, Process, Shipment, Log} from '../types';

export interface EntityHandler<T extends object> {                               
  set: (name: keyof T, value: any) => void,                                     
  remove: (name: keyof T) => void,                                              
  clear: () => void,
  reset: () => void,                                                            
  has: (key: keyof T) => boolean,
  get: <K extends keyof T>(key: K, fallback?: T[K]) => T[K],
  keys: () => Array<keyof T>;
  values: () => Array<T[keyof T]>;
  entries: () => Array<[keyof T, T[keyof T]]>;
  validate: () => Record<string, string>,                                       
  errors: Record<string, string>,                                               
}   

export interface SpecimenHandler extends EntityHandler<Specimen> {        
   setProcess: (process: 'collection' | 'preparation' | 'analysis', name: keyof Process, value: any) => void,
   setData: (process: 'collection' | 'preparation' | 'analysis', key: string, value: any) => void,
}   

export interface PoolHandler extends EntityHandler<Pool> {        
}   

export interface ContainerHandler extends EntityHandler<Container> {      
  getParentContainerBarcodes: (barcode: string, barcodes: string[]) => Promise<string[]>,
}   

export interface ShipmentHandler extends EntityHandler<Shipment> {      
  setLog: (name: keyof Log, value: any, index: number) => void,
}   
