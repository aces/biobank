/**                                                                             
 * Clone an object                                                              
 *                                                                              
 * @param {T} object - The object to clone                                      
 * @returns {T} Cloned object                                                   
 */                                                                             
export function clone<T>(object: T): T {                                        
  return JSON.parse(JSON.stringify(object));                                    
}    
