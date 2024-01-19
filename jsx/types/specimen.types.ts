import { Container } from '../types';

export enum ProcessType {
  Collection = 'collection',
  Preparation = 'preparation',
  Analysis = 'analysis',
}

export type Process = {                                                         
  centerId?: string,                                                            
  comments?: string,                                                            
  data?: { [key: string]: any },                                                
  date?: string,                                                                
  examinerId?: string,                                                          
  protocolId?: string,                                                          
  quantity?: number,                                                            
  time?: string,                                                                
  unitId?: string                                                               
};          

type ProcessMap = {
  [K in ProcessType]?: Process;         
};

export type Specimen = ProcessMap & {                                                        
  id?: string,                                                                  
  barcode?: string,                                                             
  candidateAge?: number,                                                        
  candidateId?: string,                                                         
  candidatePSCID?: string,                                                      
  centerId?: string,                                                            
  container?: Container,                                                        
  containerId?: string,                                                         
  fTCycle?: number,                                                             
  parentSpecimenBarcodes?: string[],                                            
  parentSpecimenIds?: string[],                                                 
  poolId?: string,                                                              
  poolLabel?: string
  projectIds?: string[],                                                        
  quantity?: number,                                                            
  sessionId?: string,                                                           
  typeId?: string,                                                              
  unitId?: string,                                                              
};    

