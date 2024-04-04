import { useState, useEffect } from 'react';
import { BaseAPI } from '../APIs';

interface FetchResponse<T> {
  data: T | null;
  isLoading: boolean;
  error: Error | null;
}

export const useFetch = <T>(
  api: BaseAPI<T>, 
  id: string,
  dependencies: any[] = []
): FetchResponse<T> => {

    const cacheKey = JSON.stringify(dependencies);

  if (fetchCache.has(cacheKey)) {
    const cached = fetchCache.get(cacheKey);
    if (cached.status === 'success') {
      return cached.data;
    } else if (cached.status === 'error') {
      throw cached.error;
    } else {
      // If status is 'pending', re-throw the promise
      throw cached.promise;
    }
  }

  const promise = api.getById(id)
    .then(data => {
      fetchCache.set(cacheKey, { status: 'success', data });
    })
    .catch(error => {
      fetchCache.set(cacheKey, { status: 'error', error });
      throw error;
    });

  // Store the pending promise in the cache and throw it
  fetchCache.set(cacheKey, { status: 'pending', promise });
  throw promise;
};

//   const [data, setData] = useState<T | null>(null);
//   const [isLoading, setIsLoading] = useState<boolean>(false);
//   const [error, setError] = useState<Error | null>(null);
// 
//   useEffect(() => {
//     const fetchData = async () => {
//       setIsLoading(true);
//       try {
//         const result = await api.getById();
//         setData(result);
//         setIsLoading(false);
//         setFormErrors({});
//       } catch (err) {
//         const error = err as Error;
//         setError(error);
//         setIsLoading(false);
//       }
//     };
// 
//     fetchData();
//   }, dependencies);
// 
//   return { data, isLoading, error };
// };
