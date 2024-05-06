import { IContainer, EntityHook, ContainerHook } from '../';

export interface IData extends Record<string, any> {};                                         
                                                                                
export interface IProcess {                                                         
  center: string,                                                            
  comments?: string,                                                            
  data?: IData,                                                                  
  date: string,                                                                
  examiner: string,                                                          
  protocol: string,                                                          
  quantity: number,                                                            
  time: string,                                                                
  unit: string                                                               
};                                                                              
                                                                                

// TODO: maybe change this to an enum later, having trouble currently.
// export enum ProcessType {                                                       
//   Collection = 'collection',                                                    
//   Preparation = 'preparation',                                                  
//   Analysis = 'analysis',                                                        
// }                                                                               

export type ProcessType = 'collection' | 'preparation' | 'analysis';

// export type ProcessMap = {                                                             
//   [K in ProcessType]?: IProcess;                                                 
// };                                                                              
                                                                                
export type ISpecimen = {                                           
  id: string,                                                                   
  barcode: string,                                                              
  candidateAge: number,                                                         
  candidate: string,                                                          
  container: Partial<IContainer>,                                                         
  collection: Partial<IProcess>,
  preparation?: Partial<IProcess>,
  analysis?: Partial<IProcess>,
  fTCycle: number,                                                              
  parentSpecimens?: string[],                                            
  pool?: string,                                                              
  poolLabel?: string                                                            
  projects: string[],                                                         
  quantity: number,                                                             
  session: string,                                                            
  type: string,                                                               
  unit: string,                                                               
};      
