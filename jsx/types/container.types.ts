import { ContainerHandler } from '../types';

export type Container = ContainerHandler & {                                                       
  id?: string,                                                                  
  barcode?: string,                                                             
  centerId?: string,                                                            
  childContainerIds?: string[],                                                 
  childContainerBarcodes?: Record<string, string> & { unassigned: string[] },           
  comments?: string,                                                            
  coordinate?: number,                                                          
  dimensionId?: string,                                                         
  expirationDate?: string,                                                      
  lotNumber?: string,                                                           
  parentContainerBarcode?: string,                                              
  parentContainerId?: string,                                                   
  shipmentBarcodes?: string[],                                                  
  specimenId?: string,                                                          
  statusId?: string,                                                            
  temperature?: number,                                                         
  typeId?: string,                                                              
};   
