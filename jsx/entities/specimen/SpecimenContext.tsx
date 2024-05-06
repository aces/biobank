import React, { createContext } from 'react';                        
import { useSpecimen, SpecimenHook } from './';          
import { ContainerProvider } from '../';
import { Options } from '../../types';                                             
                                                                                
export const SpecimenContext = React.createContext<SpecimenHook>(undefined);             
                                                                                
export const SpecimenProvider: React.FC<{                                        
  specimen: SpecimenHook,                                                  
}> = ({                                                                         
  children,                                                                     
  specimen,                                                                      
}) => {                                                                         
  return (                                                                      
    <SpecimenContext.Provider value={specimen}>                                   
      <ContainerProvider container={specimen.container}>
        {children}                                                                
      </ContainerProvider>
    </SpecimenContext.Provider>                                                  
  );                                                                            
};                                                                              

                                                                                
