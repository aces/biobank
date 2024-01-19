import { useState } from 'react';
import { GenericStateHandler } from '../types';
import { isEmpty } from '../utils';

/**                                                                             
 * A custom hook for managing state in a React component with generic typing.   
 *                                                                              
 * This hook initializes the state with a given initial value and provides functions
 * to set and remove properties dynamically. It's designed to be flexible and can
 * be used with any type that matches the generic `T`.                          
 *                                                                              
 * @param {T} initialState - The initial state value of type `T`.               
 *                                                                              
 * @returns A tuple containing:                                                 
 * - The current state value (`state`), which can be updated using the provided functions.
 * - An object with the following functions:                                    
 *   - `set`: A function to update a specific property in the state.            
 *     Takes a property name (key of `T`) and a value, and sets the property to that value.
 *   - `remove`: A function to remove a specific property from the state.       
 *     Takes a property name (key of `T`) and deletes it from the current state.
 *                                                                              
 * @template T The type of the state.                                           
 *                                                                              
 * @example                                                                     
 * const [state, { set, remove }] = useGenericState<{name: string, age: number}>({name: 'John', age: 30});
 * set('age', 31); // Updates age to 31                                         
 * remove('name'); // Removes the name property from the state                  
 */                                                                             
function useGenericState<T extends object>(                              
  stateInitializer: T,                                                          
  validateState: (state: T) => Record<string, string> = () => ({})              
): [T, GenericStateHandler<T>] {                                                
  const [initialState, setInitialState] = useState<T>(stateInitializer);        
  const [state, setState] = useState<T>(initialState);                          
  const [errors, setErrors] = useState<Record<string, string>>({});             
                                                                                
  /**                                                                           
   * Sets a property of the state object to a specified value.                  
   * This function takes a key and a value, and updates the state               
   * object with the given value at the specified key.                          
   *                                                                            
   * @param {keyof T} name - The key of the state object to update.             
   *                         'keyof T' ensures that the key is valid for the state object's type.
   * @param {any} value - The new value to set for the specified key in the state object.
   */                                                                           
  const set = (                                                                 
    name: keyof T,                                                              
    value: any                                                                  
  ) => {                                                                        
    setState((prevState) => ({ ...prevState, [name]: value }));                 
  };                                                                            
                                                                                
  /**                                                                           
   * Removes a property from the state object.                                  
   * This function takes a key and deletes the corresponding property from the state object.
   *                                                                            
   * @param {keyof T} name - The key of the state object's property to remove.  
   *                         'keyof T' ensures that the key is valid for the state object's type.
   */                                                                           
  const remove = (                                                              
    name: keyof T                                                               
  ) => {                                                                        
    setState((prevState) => {                                                   
      const newState = { ...prevState };                                        
      delete newState[name as keyof typeof newState];                           
      return newState;                                                          
    });                                                                         
  };                                                                            
                                                                                
  const reset = () => {                                                         
    setState(initialState);                                                     
  };                                                                            
                                                                                
  const validate = () => {                                                      
    const newErrors = validateState(state);                                     
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
  //       const updatedState = await post(state, url, 'PUT');                     
  //                                                                               
  //       // set new baseline state                                               
  //       setInitialState(updatedState);                                          
  //       setState(updatedState);                                                 
  //   } catch (error) {                                                           
  //       // Handle or rethrow the error as appropriate                           
  //       console.error(error);                                                   
  //   }                                                                           
  // }                                                                             
                                                                                
  return [state, { set, remove, reset, validate, errors }];                
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

export default useGenericState;
