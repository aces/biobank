import {                                                                           
  ICandidate,                                                                      
  IProject,                                                                        
  ICenter,                                                                         
  ISession,                                                                        
  IUnit,                                                                           
  ISpecimen,
  ISpecimenType,
} from '../';  

export interface IPool {                                                            
  id: string,                                                                   
  label: string,                                                                
  candidate: ICandidate,                                                       
  center: ICenter,                                                             
  session: ISession,                                                            
  date: string,                                                                 
  projects: IProject[],                                                         
  quantity: number,                                                             
  specimens: ISpecimen[],                                                   
  time: string,                                                                 
  type: ISpecimenType,                                                                 
  unit: IUnit,                                                               
}; 
