import BaseAPI from './BaseAPI';
import { IContainer } from '../entities';

export default class ContainerAPI extends BaseAPI<IContainer> {
  constructor(endpoint: string = '') {
    super('containers'+endpoint); // Provide the base URL for container-related API
  }
}
