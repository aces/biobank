import { useQuery, UseQueryResult, QueryKey } from 'react-query';
                                                                                
export const useRequest = <T>(                                               
  key: QueryKey,
  requestFunction: () => Promise<T>,                                            
  placeholder = {},
  dependencies: any[] = [],
): UseQueryResult<T, Error> => {

  return useQuery([key, ...dependencies], requestFunction, {
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

  // const [data, setData] = useState({});                             
  // const [isLoading, setIsLoading] = useState(true);                   
  // const [error, setError] = useState<Error>();                       
  //                                                                               
  // useEffect(() => {                                                             
  //   // Validate that requestFunction is a function
  //   if (typeof requestFunction !== 'function') {
  //     console.error('Error in useHTTPRequest: requestFunction is not a function', requestFunction);
  //     setError(new Error('requestFunction is not a function'));
  //     setIsLoading(false);
  //     return;
  //   }

  //   const fetchData = async () => {                                             
  //     try {                                                                     
  //       const result = await requestFunction();
  //       setData(result);
  //     } catch (err) {
  //       console.error('Error fetching data:', err);
  //       setError(err as Error);
  //     } finally {
  //       setIsLoading(false);
  //     }              
  //   };                                                                          
  //                                                                               
  //   fetchData();                                                                

  // }, []);                                                             
  //                                                                               
  // return { data, isLoading, error };                                            
};                                                                              
