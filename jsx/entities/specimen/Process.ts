import { Entity } from '../';                                               
import { IProcess } from './';  

export class Process extends Entity<IProcess> {
  constructor(initialData: Partial<IProcess>) {
    super(initialData);
  }

  validate(): Record<string, string> {
    const errors: Record<string, string> = {};
    // Add more validations as needed
    return errors;
  }
}
