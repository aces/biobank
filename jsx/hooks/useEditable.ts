import { useState } from 'react';

export const useEditable = () => {                                              
  const [editable, setEditable] = useState<Record<string, boolean>>({});        
                                                                                
  const edit = (name: string) => {                                              
    setEditable(prevEditable => ({                                              
      ...Object.fromEntries(Object.keys(prevEditable).map(key => [key, false])),
      [name]: !prevEditable[name]                                               
    }));                                                                        
  };                                                                            
                                                                                
  const clear = () => setEditable({});                                          
                                                                                
  // TODO: turn into tuple                                                      
  return { editable, edit, clear};                                              
}
