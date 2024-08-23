import { createContext, useContext, ReactNode } from 'react';
import { SpecimenHook } from './';
import { ContainerProvider } from '../';

const SpecimenContext = createContext<SpecimenHook | null>(null);

interface SpecimenProviderProps {
  specimen: SpecimenHook;
  children?: ReactNode;
}

export const SpecimenProvider = ({
  children,
  specimen
}: SpecimenProviderProps): JSX.Element => {
  return (
    <SpecimenContext.Provider value={specimen}>
      <ContainerProvider container={specimen.container}>
        {children}
      </ContainerProvider>
    </SpecimenContext.Provider>
  );
};

export const useSpecimenContext = (): SpecimenHook => {
  const context = useContext(SpecimenContext);

  if (context === undefined) {
    console.error('useSpecimenContext must be used within a BiobankProvider');
    throw new Error('useSpecimenContext must be used within a BiobankProvider');
  }

  return context;
};
