/**                                                                             
 * Maps an object's values based on a specific attribute.                       
 *                                                                              
 * @param {{ [key: string]: any }} object - The object to map. An object with   
 * string keys and values of any type.                                          
 * @param {string} attribute - The attribute to map by.                         
 * @returns {{ [key: string]: any }} Mapped object with the same keys as the    
 * input object and values derived from the specified attribute.                
 */                                                                             
export function mapFormOptions(                                                 
  object: { [key: string]: any },                                               
  attribute: string                                                             
): { [key: string]: string } {                                                     
  return Object.keys(object).reduce((result, id) => {                           
    result[id] = object[id][attribute];                                         
    return result;                                                              
  }, {});                                                                       
}   
