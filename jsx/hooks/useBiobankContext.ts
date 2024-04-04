import React, { useContext } from 'react';
import { BiobankContext, BiobankContextType } from '../contexts'; // Adjust the path as necessary

export const useBiobankContext = (): BiobankContextType => {
  const context = useContext(BiobankContext);

  if (context === undefined) {
    console.error('useBiobankContext must be used within a BiobankProvider');
    throw new Error('useBiobankContext must be used within a BiobankProvider');
  }

  return context;
};
