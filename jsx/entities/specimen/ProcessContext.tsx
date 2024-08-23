import { createContext, useContext, ReactNode } from 'react';
import { ProcessHook } from './';

const ProcessContext = createContext<ProcessHook | null>(null);

interface ProcessProviderProps {
  process: ProcessHook;
  children?: ReactNode;
}

export const ProcessProvider = ({
  process,
  children,
}: ProcessProviderProps): JSX.Element => {
  return (
    <ProcessContext.Provider value={process}>
       {children}
    </ProcessContext.Provider>
  );
};

export const useProcessContext = (): ProcessHook => {
  const context = useContext(ProcessContext);

  if (context === undefined) {
    console.error('useProcessContext must be used within a BiobankProvider');
    throw new Error('useProcessContext must be used within a BiobankProvider');
  }

  return context;
};
