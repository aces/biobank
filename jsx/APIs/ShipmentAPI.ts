import BaseAPI from './BaseAPI';
import { Shipment } from '../types'; // Assuming you have a User type

export default class ShipmentAPI extends BaseAPI<Shipment> {
  constructor() {
    super('/biobank/containers'); // Provide the base URL for container-related API
  }
}
