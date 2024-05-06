import BaseAPI from './BaseAPI';
import { IContainer } from '../entities';

export default class ContainerAPI extends BaseAPI<IContainer> {
  constructor() {
    super('/biobank/containers'); // Provide the base URL for container-related API
  }
}
