import BaseAPI from './BaseAPI';
import { Shipment } from '../types'; // Assuming you have a User type

class ShipmentAPI extends BaseAPI<Shipment> {
  constructor() {
    super('/biobank/containers'); // Provide the base URL for container-related API
  }
}

export default ShipmentAPI;
