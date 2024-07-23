import BaseAPI from './BaseAPI';
import { QueryParam } from './';
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

  async getStatuses(queryParam?: QueryParam): Promise<IShipmentStatus[]> {
    return this.getSubEndpoint<IShipmentStatus>([ShipmentSubEndpoint.Statuses], queryParam);
  }

  async getTypes(queryParam?: QueryParam): Promise<IShipmentType[]> {
    return this.getSubEndpoint<IShipmentType>([ShipmentSubEndpoint.Types], queryParam);
  }
}
