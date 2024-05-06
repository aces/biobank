import React, { createContext, useContext } from 'react';
import { ContainerHook, useContainer} from './';

const ContainerContext = createContext<ContainerHook>(undefined);

export const useContainerContext = (): ContainerHook => {
    return useContext(ContainerContext);
};

export const ContainerProvider: React.FC<{
  container: ContainerHook
}> = ({ container, children }) => {
  return (
    <ContainerContext.Provider value={container}>
      {children}
    </ContainerContext.Provider>
  );
};
