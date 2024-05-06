import { Entity } from '../';
import { IData } from './';

export class Data extends Entity<IData> {                                     
  constructor(initialData: Partial<IData>) {                                              
    super(initialData);                                                         
  }                                                                             
                                                                                
  validate(): Record<string, string> {                                          
    const errors: Record<string, string> = {};                                  
    if (!this.data.barcode) {                                                   
      errors.barcode = 'Barcode is required.';                                  
    }                                                                           
    // Add more validations as needed                                           
    return errors;                                                              
  }                                                                             
}     
