import BaseAPI from './BaseAPI';
import Query, { QueryParam } from './Query';
import { IContainer } from '../entities';

export enum ContainerSubEndpoint {
  Types = 'types',
  Statuses = 'statuses',
}

export default class ContainerAPI extends BaseAPI<IContainer> {
  constructor() {
    super('containers'); // Provide the base URL for container-related API
  }

  async getTypes(queryParam?: QueryParam): Promise<string[]> {
    this.setSubEndpoint(ContainerSubEndpoint.Types);
    const query = new Query();
    if (queryParam) {
      query.addParam(queryParam);
    }

    return await this.get<string>(query);
  }

  // TODO: to be updated to something more useful — status will probably no
  // longer be something that you can select but rather something that is
  // derived. 
  async getStatuses(queryParam?: QueryParam): Promise<string[]> {
    this.setSubEndpoint(ContainerSubEndpoint.Types);
    const query = new Query();
    if (queryParam) {
      query.addParam(queryParam);
    }

    return await this.get<string>(query);
  }
}
