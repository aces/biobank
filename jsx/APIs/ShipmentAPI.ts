import BaseAPI from './BaseAPI';
import { Shipment } from '../entities';

export default class ShipmentAPI extends BaseAPI<Shipment> {
  constructor() {
    super('/shipments');
  }
}
