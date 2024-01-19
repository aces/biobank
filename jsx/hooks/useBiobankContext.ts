import React, { useContext } from 'react';
import BiobankContext from '../contexts'; // Adjust the path as necessary

export const useBiobankContext = () => {
  const context = useContext(BiobankContext);

  if (context === undefined) {
    throw new Error('useBiobankContext must be used within a BiobankProvider');
  }

  return context;
};
