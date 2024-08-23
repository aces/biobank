import BaseAPI from './BaseAPI';
import Query, { QueryParam } from './Query';
import { IShipment, IShipmentStatus, IShipmentType } from '../entities';

export enum ShipmentSubEndpoint {
  Statuses = 'statuses',
  Types = 'types',
  Details = 'details'
}

export default class ShipmentAPI extends BaseAPI<IShipment> {
  constructor() {
    super('shipments');
  }

  async getStatuses(queryParam?: QueryParam): Promise<string[]> {
    this.setSubEndpoint(ShipmentSubEndpoint.Statuses);
    const query = new Query();
    if (queryParam) {
      query.addParam(queryParam);
    }

    return this.get<string>(query);
  }

  async getTypes(queryParam?: QueryParam): Promise<IShipmentType[]> {
    this.setSubEndpoint(ShipmentSubEndpoint.Types);
    const query = new Query();

    if (queryParam) {
      query.addParam(queryParam);
    }

    return this.get<IShipmentType>(query);
  }
}
