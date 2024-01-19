/**                                                                             
 * Pad a barcode.                                                               
 *                                                                              
 * @param {string} pscid - A PSCID, representing a unique identifier.           
 * @param {number} increment - The numeric value representing the amount of     
 * padding needed.                                                              
 * @returns {string} Padded barcode string.                                     
 */                                                                             
export function padBarcode(pscid: string, increment: number): string {          
  return pscid + padLeft(increment, 3);                                         
}                                                                               
                                                                                
/**                                                                             
 * Left pad a string without using a library.                                   
 *                                                                              
 * @param {number} nr - The existing number to be padded.                       
 * @param {number} n - The total desired length of the string after padding.    
 * @param {string} str - The string used for padding. Defaults to '0' if not    
 * provided.                                                                    
 * @returns {string} The padded string.                                         
 */                                                                             
function padLeft(nr: number, n: number, str: string = '0'): string {            
  return Array(n - String(nr).length + 1).join(str) + nr;                       
} 
