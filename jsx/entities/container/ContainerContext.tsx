import { createContext, useContext, ReactNode } from 'react';
import { ContainerHook } from './';

const ContainerContext = createContext<ContainerHook | null>(null);

interface ContainerProviderProps {
  container: ContainerHook;
  children?: ReactNode;
}

export const ContainerProvider = ({                                                 
  children,                                                                        
  container                                                                         
}: ContainerProviderProps): JSX.Element => {   
  return (
    <ContainerContext.Provider value={container}>
      {children}
    </ContainerContext.Provider>
  );
};

export const useContainerContext = (): ContainerHook => {
  const context = useContext(ContainerContext);
  if (context === null) {
    throw new Error('useContainerContext must be used within a ContainerProvider');
  }
  return context;
};
