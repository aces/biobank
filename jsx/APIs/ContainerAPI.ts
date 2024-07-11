import BaseAPI from './BaseAPI';
import { IContainer } from '../entities';

export default class ContainerAPI extends BaseAPI<IContainer> {
  constructor() {
    super('/containers'); // Provide the base URL for container-related API
  }
}
