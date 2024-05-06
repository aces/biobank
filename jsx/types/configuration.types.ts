import { BiobankContextType } from '../contexts';

export interface FieldConfiguration<I extends object> {                                               
  label: string,                                                                
  type: 'text' | 'select' | 'date' | 'search' | 'time' | 'textarea' | 'boolean'
  | 'number',                                             
  required?: boolean,                                                            
  disabled?: boolean,
  multiple?: boolean,
  autoSelect?: boolean, 
  placeHolder?: string,
  emptyOption?: boolean,
  getOptions?: (context: BiobankContextType) =>
  Record<string, string>,
}    
