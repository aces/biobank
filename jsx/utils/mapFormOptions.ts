/**                                                                             
 * Maps an object's values based on a specific attribute.                       
 *                                                                              
 * @param {{ [key: string]: any }} object - The object to map. An object with   
 * string keys and values of any type.                                          
 * @param {string} attribute - The attribute to map by.                         
 * @returns {{ [key: string]: any }} Mapped object with the same keys as the    
 * input object and values derived from the specified attribute.                
 */                                                                             
export function mapFormOptions<T>(                                                 
  entities: T[],                                               
  attribute: string                                                             
): Record<string, string> {                                                     
  return entities.reduce((result, entity) => {                           
    result[entity[attribute]] = entity[attribute];                                         
    return result;                                                              
  }, {});                                                                       
}   

export function mapLabel<T>(
  entities: T[],        
): { [key: string]: string } {
  return mapFormOptions(entities, 'label');        
}
