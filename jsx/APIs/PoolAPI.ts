import BaseAPI from './BaseAPI';
import { Pool } from '../entities';

export default class PoolAPI extends BaseAPI<Pool> {
  constructor() {
    super('/pools');
  }
}
