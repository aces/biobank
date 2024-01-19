import BaseAPI from './BaseAPI';
import { Pool } from '../types';

class PoolAPI extends BaseAPI<Pool> {
  constructor() {
    super('/biobank/pools'); // Provide the base URL for pool-related API
  }
}

export default PoolAPI;
