import { useState } from 'react';
import { EntityHandler } from '../types';
import { isEmpty } from '../utils';

/**                                                                             
 * A custom hook for managing entity in a React component with generic typing.   
 *                                                                              
 * This hook initializes the entity with a given initial value and provides functions
 * to set and remove properties dynamically. It's designed to be flexible and can
 * be used with any type that matches the generic `T`.                          
 *                                                                              
 * @param {T} initialState - The initial entity value of type `T`.               
 *                                                                              
 * @returns A tuple containing:                                                 
 * - The current entity value (`entity`), which can be updated using the provided functions.
 * - An object with the following functions:                                    
 *   - `set`: A function to update a specific property in the entity.            
 *     Takes a property name (key of `T`) and a value, and sets the property to that value.
 *   - `remove`: A function to remove a specific property from the entity.       
 *     Takes a property name (key of `T`) and deletes it from the current entity.
 *                                                                              
 * @template T The type of the entity.                                           
 *                                                                              
 * @example                                                                     
 * const [entity, { set, remove }] = useEntity<{name: string, age: number}>({name: 'John', age: 30});
 * set('age', 31); // Updates age to 31                                         
 * remove('name'); // Removes the name property from the entity                  
 */                                                                             
export function useEntity<T extends EntityHandler>(                              
  entityInitializer: T,                                                          
  validateEntity: (entity: T) => Record<string, string> = () => ({})              
): T {                                                
  const [initialEntity, setInitialEntity] = useState<T>(entityInitializer);        
  const [entity, setEntity] = useState<T>(initialEntity);                          
  const [errors, setErrors] = useState<Record<string, string>>({});             
                                                                                
  /**                                                                           
   * Sets a property of the entity object to a specified value.                  
   * This function takes a key and a value, and updates the entity               
   * object with the given value at the specified key.                          
   *                                                                            
   * @param {keyof T} name - The key of the entity object to update.             
   *                         'keyof T' ensures that the key is valid for the entity object's type.
   * @param {any} value - The new value to set for the specified key in the entity object.
   */                                                                           
  const set = <K extends keyof T>(                                                                 
    name: K,                                                              
    value: T[K]                                                                  
  ) => {                                                                        
    setEntity((prevEntity) => ({ ...prevEntity, [name]: value }));                 
  };                                                                            
                                                                                
  /**                                                                           
   * Removes a property from the entity object.                                  
   * This function takes a key and deletes the corresponding property from the entity object.
   *                                                                            
   * @param {keyof T} name - The key of the entity object's property to remove.  
   *                         'keyof T' ensures that the key is valid for the entity object's type.
   */                                                                           
  const remove = (                                                              
    name: keyof T                                                               
  ) => {                                                                        
    setEntity((prevEntitiy) => {                                                   
      const newEntity = { ...prevEntity };                                        
      delete newEntity[name as keyof typeof newEntity];                           
      return newEntity;                                                          
    });                                                                         
  };                                                                            
                                                                                
  // Clear all properties
  const clear = () => {
    setEntity({} as T); // Reset entity to an empty object, adjust as necessary for type T
  };

  const reset = () => {                                                         
    setEntity(initialEntity);                                                     
  };                                                                            

  // Check if a property exists
  const has = (key: keyof T) => {
    return key in entity;
  };

  // Get the value of a property, with optional fallback
  const get = <K extends keyof T>(key: K, fallback?: T[K]) => {
    return entity[key] ?? fallback;
  };

  // Get all keys as an array
  const keys = () => {
    return Object.keys(entity) as Array<keyof T>;
  };

  // Get all values as an array
  const values = () => {
    return Object.values(entity);
  };

  // Get entries as an array of [key, value] pairs
  const entries = () => {
    return Object.entries(entity) as Array<[keyof T, T[keyof T]]>;
  };
                                                                                
  const validate = () => {                                                      
    const newErrors = validateEntity(entity);                                     
    setErrors(newErrors);                                                       
    return newErrors;                                                           
  };                                                                            
                                                                                
  // const put = async (url: string): Promise<void> => {                           
  //   const newErrors = validate();                                               
  //                                                                               
  //   if (!isEmpty(newErrors)) {                                                  
  //     return Promise.reject();                                                  
  //   }                                                                           
  //                                                                               
  //   try {                                                                       
  //       const updatedState = await post(entity, url, 'PUT');                     
  //                                                                               
  //       // set new baseline entity                                               
  //       setInitialState(updatedState);                                          
  //       setEntity(updatedState);                                                 
  //   } catch (error) {                                                           
  //       // Handle or rethrow the error as appropriate                           
  //       console.error(error);                                                   
  //   }                                                                           
  // }                                                                             
                                                                                
  return {
    ...entity,
    set,
    remove,
    clear,
    reset,
    has,
    get,
    keys,
    values,
    entries,
    validate,
    errors,             
  }
}    

// // Generic get function                                                         
// async function getEntity<T>(endpoint: string, entityType: string): Promise<T> { 
//   try {                                                                         
//     const url = `${loris.BaseURL}/biobank/${endpoint}`;                         
//     const [data] = await get(url);                                              
//     if (!data) {                                                                
//       throw new Error(`Failed to fetch ${entityType} data`);                    
//     }                                                                           
//     return data as T;                                                           
//   } catch (error) {                                                             
//     console.error(`Error fetching ${entityType}:`, error);                      
//     throw error;                                                                
//   }                                                                             
// }  
