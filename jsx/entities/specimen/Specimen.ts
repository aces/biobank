import { Entity } from '../';                                               
import { ISpecimen } from './';  

export class Specimen extends Entity<ISpecimen> {                             
  constructor(initialData: Partial<ISpecimen>) {                                          
    super(initialData);                                                         
  }                                                                             
                                                                                
  validate() {                                          
    const errors: Partial<Record<keyof ISpecimen, string>> = {};                                  
    if (!this.data.barcode) {                                                   
      errors.barcode = 'Barcode is required.';                                  
    }                                                                           
    // Add more validations as needed                                           
    return errors;                                                             
  }                                                                             
}                                                                               
