import {
  ICenter,        
  IUnit,
  IShipment
} from '../';

export type IDimension = {                                                       
  y: number,                                                                    
  x: number,                                                                    
  z: number,                                                                    
  xNum: boolean,                                                                 
  yNum: boolean,                                                                 
  zNum: boolean,                                                                 
}; 

export type ICapacity = {
  quantity: number,
  unit: IUnit,  
}

export type IContainerType = {
  label: string,
  brand: string,
  productNumber: string,
  dimension: IDimension,
  capacity: ICapacity,  
}

export interface IContainer {                                    
  id: string,                                                                  
  barcode: string,                                                             
  center: Partial<ICenter>,                                                            
  children?: IContainer[],
  comments?: string,                                                            
  coordinate?: number,                                                          
  dimension: IDimension,                                                         
  expirationDate?: string,                                                      
  lotNumber?: string,                                                           
  parent?: IContainer,                                              
  shipments?: IShipment[],                                                  
  status?: string,                                                            
  temperature: number,                                                         
  type: IContainerType,                                                              
};     
