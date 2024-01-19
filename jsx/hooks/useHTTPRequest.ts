import { useState, useEffect } from 'react';                                    
                                                                                
interface HTTPResponse<T> {                                                    
  data: T | null;                                                               
  isLoading: boolean;                                                           
  error: Error | null;                                                          
}                                                                               
                                                                                
const useHTTPRequest = <T>(                                                           
  requestFunction: () => Promise<T>,                                            
  dependencies: any[] = []                                                      
): HTTPResponse<T> => {                                                        
  const [data, setData] = useState<T | null>(null);                             
  const [isLoading, setIsLoading] = useState<boolean>(false);                   
  const [error, setError] = useState<Error | null>(null);                       
                                                                                
  useEffect(() => {                                                             
    const fetchData = async () => {                                             
      setIsLoading(true);                                                       
      try {                                                                     
        const result = await requestFunction();                                 
        setData(result);                                                        
        setIsLoading(false);                                                    
      } catch (err) {                                                           
        const error = err as Error;                                             
        setError(error);                                                        
        setIsLoading(false);                                                    
      }                                                                         
    };                                                                          
                                                                                
    fetchData();                                                                
  }, dependencies);                                                             
                                                                                
  return { data, isLoading, error };                                            
};                                                                              
                                                                                
export default useHTTPRequest;
