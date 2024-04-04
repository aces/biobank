import { useState, useEffect } from 'react';                                    
                                                                                
interface HTTPResponse<T> {                                                    
  data: T | null;                                                               
  isLoading: boolean;                                                           
  error: Error | null;                                                          
}                                                                               
                                                                                
export const useHTTPRequest = <T>(                                                           
  requestFunction: () => Promise<T>,                                            
  dependencies: any[] = []                                                      
): HTTPResponse<T> => {                                                        
  const [data, setData] = useState<T | null>(null);                             
  const [isLoading, setIsLoading] = useState<boolean>(true);                   
  const [error, setError] = useState<Error | null>(null);                       
                                                                                
  useEffect(() => {                                                             
    // Validate that requestFunction is a function
    if (typeof requestFunction !== 'function') {
      console.error('Error in useHTTPRequest: requestFunction is not a function', requestFunction);
      setError(new Error('requestFunction is not a function'));
      setIsLoading(false);
      return;
    }

    const fetchData = async () => {                                             
      try {                                                                     
        const result = await requestFunction();
        setData(result);
        setIsLoading(false);
      } catch (err) {
        console.error('Error fetching data:', err);
        setError(err as Error);
        setIsLoading(false);
      }                                                                         
    };                                                                          
                                                                                
    fetchData();                                                                
  }, dependencies);                                                             
                                                                                
  return { data, isLoading, error };                                            
};                                                                              
