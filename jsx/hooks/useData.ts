import { Data } from '../types';
import { useEntity } from '../hooks';

export function useData(
  initialPool: Partial<Data> = {}
): Data {

  const validateData = (data: Data): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const data = useEntity<Data>(initialData, validateData);

  return {
    ...data,
  };
}
