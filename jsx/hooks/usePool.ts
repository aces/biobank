import { Pool } from '../types';
import { useEntity, EntityHandler } from '../hooks';

export function usePool(
  initialPool: Partial<Pool> = {}
): Pool {

  const validatePool = (pool: Pool): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const pool = useEntity<Pool>(initialPool, validatePool);

  return {
    ...pool,
  };
}
