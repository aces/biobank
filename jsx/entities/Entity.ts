export abstract class Entity<I extends object> {                                           
  protected data: Partial<I>;                                                            
  protected initialData: Partial<I>;                                                     
  protected errors: Partial<Record<keyof I, string>> = {};                                
                                                                                
  constructor(initialData: Partial<I>) {                                                 
    this.data = initialData;                                                    
    this.initialData = initialData;                                             
  }                                                                             

  protected clone(newData: Partial<I> = {}): this {
    const clone = Object.create(this.constructor.prototype) as this;
    // Assuming a method to initialize or copy properties to the cloned instance
    clone.initialize(newData);
    return clone;
  }

  // Example initialization method that you might define in your class
  protected initialize(data: Partial<I>): void {
    this.data = data;
    // Initialize or copy other properties as necessary
  }
                                                                                
  // Abstract method to perform validation. Subclasses must implement this.     
  abstract validate(): Partial<Record<keyof I, string>>;                                  

  // Method to update a property in an immutable way.                           
  set(key: keyof I, value: I[keyof I]): this {                      
    return this.clone({...this.data, [key]: value});                                         
  }                                                                             
                                                                                
  // Method to remove a property in an immutable way.                           
  remove(key: keyof I): this {                                
    const { [key]: _, ...rest } = this.data;                                    
    return this.clone(rest as I);                                       
  }                                                                             

  replace(data: Partial<I>): this {
    return this.clone(data);
  }

  // Method to clear the data in an immutable way.                              
  clear(): this {                                                          
    return this.clone();                                         
  }                                                                             
                                                                                
  // Method to reset the data to its initial state in an immutable way.         
  reset(): this {                                                          
    return this.clone(this.initialData);                                
  }                                                                             
                                                                                
  // Method to get the current state of data.                                   
  getData(): Partial<I> {                                                                
    return this.data;                                                           
  }                                                                             
                                                                                
  // Method to get the current errors.                                          
  getErrors(): Partial<Record<keyof I, string>> {                                         
    return this.errors;                                                         
  }                                                                             
}

  // protected addToArray(key: keyof I, newValue: any): this {
  //   if (!Array.isArray(this.data[key])) {
  //     throw new Error(`${String(key)} is not an array.`);
  //   }

  //   const newArray = [...this.data[key], newValue];

  //   return this.clone({ ...this.data, [key]: newArray });
  // }

  // // Generic method to update arrays in an immutable way
  // protected updateArray(key: keyof I, index: number, newValue: any): this {
  //   if (!Array.isArray(this.data[key])) {
  //       throw new Error(`${key.toString()} is not an array.`);
  //   }

  // // Create a new array with the specified value updated at the given index
  //   const newArray = this.data[key].map((item, idx) => idx === index ? newValue : item);
  //   return this.clone({ ...this.data, [key]: newArray });
  // }

  // protected removeFromArray(key: keyof I, index: number): this {
  //   if (!Array.isArray(this.data[key])) {
  //     throw new Error(`${String(key)} is not an array.`);
  //   }

  //   const newArray = this.data[key].filter((_, idx) => idx !== index);

  //   return this.clone({ ...this.data, [key]: newArray });
  // }
                                                                                
    
