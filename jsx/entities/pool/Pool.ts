import { IPool } from './';
import { Entity } from '../';

export class Pool extends Entity<IPool> {                                         
  constructor(initialData: Partial<IPool>) {                                              
    super(initialData);                                                         
  }                                                                             
                                                                                
  validate() {                                          
    const errors: Partial<Record<keyof IPool, string>> = {};                                  
    if (!this.data.label) {                                                     
      errors.label = 'Label is required.';                                      
    }                                                                           
    // Add more validations as needed                                           
    return errors;                                                              
  }                                                                             
}  
