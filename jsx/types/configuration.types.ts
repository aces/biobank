import { BiobankContextType } from '../contexts';

// Utility type to extract the element type if it's an array, otherwise just use the type itself
type ElementType<T> = T extends Array<infer U> ? U : T;

export interface FieldConfiguration<I extends object, K extends keyof I> {                                               
  label: string,                                                                
  type: 'text' | 'select' | 'date' | 'search' | 'time' | 'textarea' | 'boolean'
  | 'number' | 'static' | 'input',                                             
  required?: boolean,                                                            
  disabled?: boolean,
  multiple?: boolean,
  autoSelect?: boolean, 
  placeholder?: string,
  emptyOption?: boolean,
  getOptions?: (context: BiobankContextType) => ElementType<I[K]>[],
  format?: (object: ElementType<I[K]>) => string,
}    
