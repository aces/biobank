import { useQuery, UseQueryResult, QueryKey } from 'react-query';
import { useRef } from 'react';
                                                                                
export const useRequest = <T>(
  requestFunction: () => Promise<T[]>,
  placeholder: T[] = [],
  dependencies: any[] = []
//): UseQueryResult<T, Error> => {
) => {
    // Generate a stable unique identifier for the hook's lifetime
  const uniqueIdRef = useRef(`key-${Math.random().toString(36).substr(2, 9)}`);

  // Generate a unique key based on the request function and dependencies
  const generatedKey = dependencies.length
    ? JSON.stringify(dependencies)
    : requestFunction.name || uniqueIdRef.current;

  // data: The data returned from the query function. If the query is still loading, this will be undefined unless you've set a placeholderData or initialData.

  // error: An error object if the query fails. If there is no error, this will be null.
  // 
  // isLoading: A boolean indicating whether the query is in the loading state (i.e., still fetching data).
  // 
  // isError: A boolean that is true if there was an error while fetching the data.
  // 
  // isSuccess: A boolean that is true if the query was successful and data was fetched.
  // 
  // isIdle: A boolean indicating that the query is in an idle state, typically before the query has been executed.
  // 
  // status: A string indicating the current status of the query. This can be 'idle', 'loading', 'error', or 'success'.
  // 
  // refetch: A function that can be called to manually refetch the query.
  // 
  // dataUpdatedAt: A timestamp indicating when the data was last fetched.
  // 
  // errorUpdatedAt: A timestamp indicating when the error occurred.
  const { data, isLoading, error } = useQuery([generatedKey, ...dependencies], requestFunction, {
    placeholderData: placeholder,
    refetchOnWindowFocus: false,
    onError: (error) => {
      console.error('Error fetching data:', error);
    },

    // Additional options to customize behavior:
    // refetchInterval: false, // for periodic refetching
    // staleTime: 1000 * 60 * 5, // 5 minutes of fresh data
    // cacheTime: 1000 * 60 * 60 * 24, // cache old data for a day
    // retry: 1, // Number of retry attempts on failure
  });

  return data;
};                                                                              
