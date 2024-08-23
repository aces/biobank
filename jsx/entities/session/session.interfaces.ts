import {
  ICenter
} from '../';

export interface ISession {                                                            
  id: string,                                                                   
  label: string,
  center: Partial<ICenter>,                                                       
}; 
