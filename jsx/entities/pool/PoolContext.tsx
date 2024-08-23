import { createContext, useContext, ReactNode } from 'react';
import { PoolHook } from './';

const PoolContext = createContext<PoolHook | null>(null);

interface PoolProviderProps {
  pool: PoolHook;
  children?: ReactNode;
}

export const PoolProvider = ({                                                 
  children,                                                                        
  pool                                                                         
}: PoolProviderProps): JSX.Element => {   
  return (
    <PoolContext.Provider value={pool}>
      {children}
    </PoolContext.Provider>
  );
};

export const usePoolContext = (): PoolHook => {
  const context = useContext(PoolContext);
  if (context === null) {
    throw new Error('usePoolContext must be used within a PoolProvider');
  }
  return context;
};
