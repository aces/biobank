import {
  ICenter,
  IContainer,
  IUser,
} from '../'

export interface IShipmentType {
  label: string,        
}

export interface IShipmentStatus {
  label: string,
}

export interface IShipment {                             
  id: string,                                                                   
  barcode: string,                                                              
  type: IShipmentType,                                                                 
  status: IShipmentStatus,                                                               
  active: boolean,                                                              
  originCenter: ICenter,                                                       
  destinationCenter: ICenter,                                                  
  logs: Partial<ILog>[],                                                                  
  containers: IContainer[],                                                  
}                                                                               
                                                                                
export interface ILog {                                                          
  barcode: string;                                                              
  center: ICenter;                                                             
  status: IShipmentStatus;                                                               
  user: IUser;                                                                 
  temperature?: number;                                                         
  date: string;                                                                 
  time: string;                                                                 
  comments?: string;                                                            
}       
