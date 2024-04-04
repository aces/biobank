import { PoolHandler } from '../types;

export type Pool = PoolHandler & {                                                            
  id: string,                                                                   
  candidateId: string,                                                          
  candidatePSCID: string,                                                       
  centerId: string,                                                             
  date: string,                                                                 
  label: string,                                                                
  projectIds: string[],                                                         
  quantity: number,                                                             
  sessionId: string,                                                            
  specimenBarcodes: string[],                                                   
  specimenIds: string[],                                                        
  time: string,                                                                 
  typeId: string,                                                                 
  unitId: string,                                                               
}; 
