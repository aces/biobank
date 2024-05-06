export interface IShipment {                             
  id: string,                                                                   
  barcode: string,                                                              
  type: string,                                                                 
  status: string,                                                               
  active: boolean,                                                              
  originCenter: string,                                                       
  destinationCenter: string,                                                  
  logs: Partial<ILog>[],                                                                  
  containers: string[],                                                  
}                                                                               
                                                                                
export interface ILog {                                                          
  barcode: string;                                                              
  center: string;                                                             
  status: string;                                                               
  user: string;                                                                 
  temperature?: number;                                                         
  date: string;                                                                 
  time: string;                                                                 
  comments?: string;                                                            
}       
