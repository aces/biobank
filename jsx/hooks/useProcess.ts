import { Process } from '../types';
import { useEntity, useData } from '../hooks';

export function useProcess(
  initialPool: Partial<Process> = {}
): Process {

  const validateProcess = (pool: Pool): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const process = useEntity<Process>(initialProcess, validateProcess);
  const data = useData(process.data || {});

  // if the protocol changes, the data should be reset
  useEffect(() => {
     data.clear();
  }, [process.protocolId]);

  return {
    ...process,
    data,
  };
}
