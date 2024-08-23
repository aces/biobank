import { useState, useEffect, useRef, useCallback } from 'react';
import { BaseAPI } from '../APIs';
import { useRequest } from './';

export interface Stream<T> {
  data: T[];
  progress: number;
  error: any;
  initialize: () => void;
  initialized: boolean;
  addEntity: (row: T) => void;
  updateEntity: (index: number, newEntity: T) => void;
}

// export const useStream = <T>(api: BaseAPI<T>): Stream<T> => {
//   const [data, setData] = useState<Record<string, T> | undefined>({});
//   const [progress, setProgress] = useState(0);
//   const [initialized, setInitialized] = useState(false);
//   const abortController = useRef(new AbortController());
// 
//   const initialize = useCallback(() => {
//     if (!initialized) {
//       api.fetchStream(setProgress, setData, abortController.current.signal)
//         .then(fetchedData => setData(fetchedData))
//         .finally(() => setInitialized(true));
//     }
//   }, [initialized]);  
// 
//   useEffect(() => {
//     return () => {
//       abortController.current.abort();
//     };
//   }, []);
// 
//   return { data, progress, error, initialize, initialized };
// };

export const useStream = <T>(api: BaseAPI<T>): Stream<T> => {
  const [data, setData] = useState<T[]>([]);
  const [progress, setProgress] = useState(0);
  const [error, setError] = useState<any>(null);
  const [initialized, setInitialized] = useState(false);
  const abortController = useRef(new AbortController());

  const addEntity = useCallback((row: T) => {
    setData(prevRows => [...prevRows, row]);
  }, []);

  const updateEntity = useCallback((index: number, newEntity: T) => {
    setData(prevData => {
      const updatedData = [...prevData];
      updatedData[index] = newEntity;  // Update the entity at the specific index
      return updatedData;
    });
  }, []);  

  const initialize = useCallback(() => {
    if (!initialized) {
      api.fetchStream(addEntity, setProgress, abortController.current.signal)
        .then(setInitialized(true))
        .catch(setError);  // Handle potential errors
    }
  }, [initialized]);

  useEffect(() => {
    return () => {
      abortController.current.abort();
    };
  }, []);

  return { data, progress, error, initialize, initialized, addEntity,
          updateEntity};
};
