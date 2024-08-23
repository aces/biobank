import BaseAPI from './BaseAPI';
import Query, { QueryParam } from './Query';
import { ISpecimen, ISpecimenType } from '../entities';

export enum SpecimenSubEndpoint {
  Types = 'types',
  Protocols = 'protocols',
}

export default class SpecimenAPI extends BaseAPI<ISpecimen> {
  constructor() {
    super('specimens');
  }

  //TODO: the new Query() should not be repeated across functions. These
  //functions are great but should avoid repetition.
  async getTypes(queryParam?: QueryParam): Promise<string[]> {
    this.setSubEndpoint(SpecimenSubEndpoint.Types);
    const query = new Query();
    if (queryParam) {
      query.addParam(queryParam);
    }

    return this.get<string>(query);
  }

  async getProtocols(queryParam?: QueryParam): Promise<string[]> {
    this.setSubEndpoint(SpecimenSubEndpoint.Protocols);
    const query = new Query();
    if (queryParam) {
      query.addParam(queryParam);
    }

    return this.get<string>(query);
  }
}
