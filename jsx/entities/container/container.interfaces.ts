export type Dimension = {                                                       
  y: number,                                                                    
  x: number,                                                                    
  z: number,                                                                    
  xNum: number,                                                                 
  yNum: number,                                                                 
  zNum: number,                                                                 
}; 

export interface IContainer {                                    
  id: string,                                                                  
  barcode: string,                                                             
  center: string,                                                            
  childContainers?: Record<string, string> & { unassigned: string[] },   
  comments?: string,                                                            
  coordinate?: number,                                                          
  dimension: Dimension,                                                         
  expirationDate?: string,                                                      
  lotNumber?: string,                                                           
  parentContainer?: string,                                              
  shipments?: string[],                                                  
  status?: string,                                                            
  temperature: number,                                                         
  type: string,                                                              
};     
