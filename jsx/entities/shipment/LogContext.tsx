import { createContext, useContext, ReactNode } from 'react';
import { LogHook } from './';

const LogContext = createContext<LogHook | null>(null);

interface LogProviderProps {
  log: LogHook;
  children?: ReactNode;
}

export const LogProvider = ({                                                 
  children,                                                                        
  log                                                                         
}: LogProviderProps): JSX.Element => {   
  return (
    <LogContext.Provider value={log}>
      {children}
    </LogContext.Provider>
  );
};

export const useLogContext = (): LogHook => {
  const context = useContext(LogContext);
  if (context === null) {
    throw new Error('useLogContext must be used within a LogProvider');
  }
  return context;
};
