import BaseAPI from './BaseAPI';
import { IPool } from '../entities';

export default class PoolAPI extends BaseAPI<IPool> {
  constructor() {
    super('pools');
  }
}
