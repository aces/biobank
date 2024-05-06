import { IPool, Pool } from './';
import { useEntity, EntityHook } from '../';

export interface PoolHook extends EntityHook<Pool, IPool> {
};

export function usePool(
  initialPool: Partial<IPool> = {}
): PoolHook {

  return useEntity<Pool, IPool>(() => new Pool(initialPool))
}
