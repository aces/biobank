import BaseAPI from './BaseAPI';
import { Pool } from '../types';

export default class PoolAPI extends BaseAPI<Pool> {
  constructor() {
    super('/biobank/pools'); // Provide the base URL for pool-related API
  }
}
