import React, { useState, ReactElement } from 'react';
import { Link, RouteComponentProps } from 'react-router-dom';
import {
  useSpecimen,
  useContainer,
  SpecimenProvider,
} from '../entities';
import { useBarcodePageContext, useBiobankContext} from '../hooks';
import { Globals, Header, ProcessPanel } from '../components';
import { BarcodePageProvider } from '../contexts';

export const SpecimenPage: React.FC<RouteComponentProps<{ barcode: string }>> = ({
  match,
  location,
  history
}) => {
  const { specimens } = useBiobankContext();
  const specimenHook = useSpecimen(specimens[match.param.barcode]);
  const { clear } = useBarcodePageContext();

  function clearAll() {
    // specimen.reset();
    // container.reset();
    clear();
  }

  return (
    <SpecimenProvider specimen={specimenHook}>
      <BarcodePageProvider>
        <Link to={`/`}>
          <span className='glyphicon glyphicon-chevron-left'/>
          Return to Filter
        </Link>
        <Header clearAll={clearAll}/>
        <div className='summary'>
          <Globals clearAll={clearAll}/>
          <div className="processing">
            <ProcessPanel process={'collection'} clearAll={clearAll}/>
            <ProcessPanel process={'preparation'} clearAll={clearAll}/>
            <ProcessPanel process={'analysis'} clearAll={clearAll}/>
          </div>
        </div>
      </BarcodePageProvider>
    </SpecimenProvider>
  );
}
