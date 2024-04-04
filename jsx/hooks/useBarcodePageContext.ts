import React, { useContext } from 'react';
import { BarcodePageContext } from '../contexts'; // Adjust the path as necessary

export const useBarcodePageContext = () => {
  const context = useContext(BarcodePageContext);

  if (context === undefined) {
    throw new Error('useBarcodePageContext must be used within a BarcodePageProvider');
  }

  return context;
};
