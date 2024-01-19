import React, { createContext, ReactNode } from 'react';
import { useEditable } from '../hooks';

// TODO: bring context to another file:                                         
interface BarcodePageContextType {                                              
  editable: Record<string, boolean>,                                            
  edit?: (name: string) => void,                                                
  clear?: () => void,                                                           
}                                                                               
                                                                                
// TODO: I really don't like this 'default value'                               
const defaultValue: BarcodePageContextType = {                                  
  editable: {},                                                                 
  edit: undefined,                                                              
  clear: undefined,                                                             
}                                                                               

const BarcodePageContext = React.createContext<BarcodePageContextType>(defaultValue);

interface BarcodePageProviderProps {
  children: ReactNode;
}

export const BarcodePageProvider: React.FC<BarcodePageProviderProps> = ({ children }) => {
  const {editable, edit, clear} = useEditable();

  return (
    <BarcodePageContext.Provider value={{ editable, edit, clear }}>
      {children}
    </BarcodePageContext.Provider>
  );
};
                                                                                
export default BarcodePageContext;
