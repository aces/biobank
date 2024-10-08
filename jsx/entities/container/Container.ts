import { IContainer } from './';                                                      
import { ContainerAPI } from '../../APIs';
import { Entity } from '../';
                                                                                
export class Container extends Entity<IContainer> {                                         
  constructor(initialData: Partial<IContainer>) {
    super(initialData);                                                         
  }                                                                             
                                                                                
  validate() {
    let errors: { [key: string]: string } = {};

    const required = [
      'barcode',
      'type',
      'temperature',
      // 'status',
      'center',
    ];

    const float = ['temperature'];
    console.log(this);

    // Check required fields
    required.forEach((field) => {
      const value = this.data[field];
      if (value === undefined || (typeof value === 'string' && !value.trim())) {
        errors[field] = 'This field is required!';
      }
    });

    // Validate floating-point fields
    float.forEach((field) => {
      const value = parseFloat(this.data[field]);
      if (isNaN(value) || value === Math.trunc(value)) {
        errors[field] = 'This field must be a floating-point number!';
      }
    });    
 
    // const barcodeSet = new Set(this.state.data.containers.map(c => c.id !== container.id && c.barcode));
    // if (barcodeSet.has(container.barcode)) {
    //     errors.barcode = 'Barcode must be unique.';
    // }

    console.log(errors);
 
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
