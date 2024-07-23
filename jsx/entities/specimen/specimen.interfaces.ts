import {
  IContainer,
  ICandidate,
  IProject,
  IExaminer,
  ICenter,
  ISession,
  IPool,
  IUnit,
  EntityHook,
  ContainerHook
} from '../';

export interface IData extends Record<string, any> {};                                         

export interface IAttribute {
  id: number,
  label: string,
  required: boolean,        
  show: boolean,
  datatype: string,
}

export interface IProtocol {
  label: string,
  type: ISpecimenType,
  process: ProcessType,
  attributes: IAttribute[],
}
                                                                                
export interface IProcess {                                                         
  center: Partial<ICenter>,                                                            
  comments?: string,                                                            
  data?: IData,                                                                  
  date: string,                                                                
  examiner: Partial<IExaminer>,                                                          
  protocol: Partial<IProtocol>,                                                          
  quantity: number,                                                            
  time: string,                                                                
  unit: IUnit,                                                               
};                                                                              

export interface ISpecimenType {
  label: string,
  units: IUnit[]
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
  candidate: Partial<ICandidate>,                                                          
  container: Partial<IContainer>,                                                         
  collection: Partial<IProcess>,
  preparation?: Partial<IProcess>,
  analysis?: Partial<IProcess>,
  fTCycle: number,                                                              
  parents?: Partial<ISpecimen>[],                                            
  pool?: Partial<IPool>,                                                              
  projects: Partial<IProject>[],                                                         
  quantity: number,                                                             
  session: Partial<ISession>,                                                            
  type: ISpecimenType,                                                               
  unit: IUnit,
};      
