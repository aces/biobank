import { useEntity } from '../hooks';
import { Container } from '../types';
import { ContainerAPI } from '../APIs';

function isValidContainer(container: any): container is Container {
  // Example validation: check if container has a specific property expected in a valid container
  return typeof container === 'object' && container !== null && 'barcode' in container;
}

/**                                                                             
 * Custom hook for managing container data within a React component.            
 * This hook is built on top of the useEntity hook, providing state management
 * specifically tailored for Container objects.                                 
 *                                                                              
 * @param {Container} initialContainer - Initial data for the container.        
 * @returns An object containing container state and function to modify it.     
 *                                                                              
 * @example                                                                     
 * const { container, set, remove } = useContainer(initialContainerData);       
 * set('volume', 100); // Sets the volume property to 100                       
 * remove('label'); // Removes the label property from the container state      
 */                                                                             
export function useContainer(                                                   
  initialContainer: Partial<Container> = {}                                              
): Container {                                              
  if (!isValidContainer(initialContainer)) {
    console.error('useContainer: Received data is not a valid container object.', initialContainer);
  }

  const validateContainer = (container: Container): Record<string, string> => { 
    let errors: Record<string, string> = {};                                    
                                                                                
    const required = [                                                          
      'barcode',                                                                
      'typeId',                                                                 
      'temperature',                                                            
      'statusId',                                                               
      'centerId',                                                               
    ];                                                                          
                                                                                
    const float = [                                                             
      'temperature',                                                            
    ];                                                                          
                                                                                
    // required.forEach((field) => {                                            
    //     if (!container[field]?.trim()) {                                     
    //         errors[field] = 'This field is required!';                       
    //     }                                                                    
    // });                                                                      
                                                                                
    // float.forEach((field) => {                                               
    //     const value = parseFloat(container[field]);                          
    //     if (isNaN(value) || value === Math.trunc(value)) {                   
    //         errors[field] = 'This field must be a floating-point number!';   
    //     }                                                                    
    // });                                                                      
                                                                                
    // const barcodeSet = new Set(this.state.data.containers.map(c => c.id !== container.id && c.barcode));
    // if (barcodeSet.has(container.barcode)) {                                 
    //     errors.barcode = 'Barcode must be unique.';                          
    // }                                                                        
                                                                                
    return errors;                                                              
  }                                                                             
                                                                                
                                                                                
  const container = useEntity<Container>(initialContainer, validateContainer);          
                                                                                
  // const url = `${loris.BaseURL}/biobank/containers/${container.barcode}`;       
  // const containerPut = async (url: string): Promise<void> => {                  
  //   await put(url);                                                             
  // }                                                                             
                                                                                
                                                                                
  // TODO: implement Coordinate display as well.                                
  /**                                                                           
   * Recursively retrieves the barcodes of a container and its parent containers.
   *                                                                            
   * @param {string} container - The container to start from.                   
   * @param {string[]} barcodes - Accumulator for barcodes. Defaults to empty.  
   * @returns {string[]} - Array of barcodes from the given container upwards.  
   */                                                                           
  async function getParentContainerBarcodes(                                          
    barcode: string = '',                                                       
    barcodes: string[] = []                                                     
  ): Promise<string[]> {                                                        
                                                                                
    const currentContainer = await ContainerAPI.getById(barcode);                       
    barcodes.push(currentContainer.barcode);                                    
                                                                                
    if (currentContainer.parentContainerBarcode) {                              
      await getParentContainerBarcodes(currentContainer.parentContainerBarcode, barcodes);
    }                                                                           
                                                                                
    return barcodes.reverse();                                                  
  }                                                                             
                                                                                
  return {                                                                      
    ...container,                                                                  
    getParentContainerBarcodes                                                
  }                                                                           
}    
