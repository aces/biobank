import BaseAPI from './BaseAPI';
import { Container } from '../types'; // Assuming you have a User type

class ContainerAPI extends BaseAPI<Container> {
  constructor() {
    super('/biobank/containers'); // Provide the base URL for container-related API
  }
}

export default ContainerAPI;
