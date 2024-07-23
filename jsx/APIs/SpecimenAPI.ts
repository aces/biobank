import BaseAPI from './BaseAPI';
import { ISpecimen, ISpecimenType } from '../entities';

export default class SpecimenAPI extends BaseAPI<ISpecimen> {
  constructor() {
    super('specimens');
  }

  async getTypes(parentType?: string): Promise<ISpecimenType[]> {
    const path = parentType ? `${this.baseUrl}/types/${parentType}` : `${this.baseUrl}/types`;
    return this.get<ISpecimenType>(path);
  }
}
