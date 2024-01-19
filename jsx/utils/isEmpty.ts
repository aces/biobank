/**                                                                             
 * Check if an object is either null or an empty object.                        
 *                                                                              
 * @param {object | null} object - The variable to check. It can be any object  
 * or null.                                                                     
 * @returns {boolean} True if the object is null or an empty object, false      
 * otherwise.                                                                   
 */                                                                             
export function isEmpty(object: object | null): boolean {                       
  if (object == null) {                                                         
    return true;                                                                
  }                                                                             
                                                                                
  for (let prop in object) {                                                    
    if (object.hasOwnProperty(prop)) {                                          
      return false;                                                             
    }                                                                           
  }                                                                             
                                                                                
  return JSON.stringify(object) === JSON.stringify({});                         
}  
