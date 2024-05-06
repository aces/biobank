import { IContainer } from './';                                                      
import { ContainerAPI } from '../../APIs';
import { Entity } from '../';
                                                                                
export class Container extends Entity<IContainer> {                                         
  constructor(initialData: Partial<IContainer>) {                                              
    super(initialData);                                                         
  }                                                                             
                                                                                
  validate() {
    let errors = {};
 
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

  /**
   * Recursively retrieves the barcodes of a container and its parent containers.
   *
   * @param {string} container - The container to start from.
   * @param {string[]} barcodes - Accumulator for barcodes. Defaults to empty.
   * @returns {string[]} - Array of barcodes from the given container upwards.
   */
  async getParentContainerBarcodes(
    barcode: string = '',
    barcodes: string[] = []
  ): Promise<string[]> {

    const currentContainer = await ContainerAPI.getById(barcode);
    barcodes.push(currentContainer.barcode);

    if (currentContainer.parentContainerBarcode) {
      await this.getParentContainerBarcodes(currentContainer.parentContainerBarcode, barcodes);
    }

    return barcodes.reverse();
  }
}      
