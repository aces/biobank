import { IPool, Pool } from './';
import { useEntity, EntityHook } from '../';

export interface PoolHook extends EntityHook<Pool, IPool> {
};

export function usePool(
  initialPool?: Pool
): PoolHook {
  const instance = initialPool || new Pool({});        

  return useEntity<Pool, IPool>(instance);
}
