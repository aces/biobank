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
     //   const stati = options.container.stati;
 //   const dispensedId = Object.keys(stati)
 //   .find(
 //      (key) => stati[key].label === 'Dispensed'
 //      );
 //   const update = Object.values(list)
 //   .reduce((result, item) => {
 //     item.container.statusId = dispensedId;
 //     item.specimen.quantity = '0';
 //     // XXX: By updating the container and specimen after, it's causing issues
 //     // if they don't meet validation. The error is being thrown only after the
 //     // pool has already been saved to the atabase! Not sure how to resolve this.
 //     // return [...result,
 //     //         () => this.updateContainer(item.container, false),
 //     //         () => this.updateSpecimen(item.specimen, false),
 //     //       ];
 //   }, []);

 //   // const errors = this.validatePool(pool);
 //   // if (!isEmpty(errors)) {
 //   //   return Promise.reject(errors);
 //   // }

 //   const pools = await post(pool, poolAPI, 'POST');
 //   setPools(pools);
  }                                                                             
}  
