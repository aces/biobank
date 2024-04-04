import ReactDOM from 'react-dom';
import { BrowserRouter, Route, Routes } from 'react-router-dom';
import { useState, useEffect} from 'react';
import { Tabs, TabPane } from 'Tabs';
import {
  ContainerPage,
  SpecimenPage,
  SpecimenTab, 
  ContainerTab, 
  PoolTab, 
  // ShipmentTab, 
} from '../components';
import {
  OptionAPI,
  SpecimenAPI,
  ContainerAPI,
  PoolAPI,
  ShipmentAPI
} from '../APIs';
import { BiobankProvider, BarcodePageProvider } from '../contexts';
import { useHTTPRequest } from '../hooks';
declare const loris: any;

/**
 * The main React entrypoint for the biobank module. This component
 * renders the index page.
 */
function BiobankIndex() {

  const { data: options, isLoading: optWait, error: optionsError }
    = useHTTPRequest(() => new OptionAPI().getAll());

  const { data: containers, isLoading: contWait, error: containersError }
    = useHTTPRequest(() => new ContainerAPI().getAll());

  const { data: specimens, isLoading: specWait, error: specimensError }
    = useHTTPRequest(() => new SpecimenAPI().getAll());

  const { data: pools, isLoading: poolWait, error: poolsError }
    = useHTTPRequest(() => new PoolAPI().getAll());

  const { data: shipments, isLoading: shipWait, error: shipmentsError }
    = useHTTPRequest(() => new ShipmentAPI().getAll());

  if (contWait || specWait || poolWait || shipWait || optWait) {
    return <div>Loading...</div>; // Render a loader
  }

  if (containersError || specimensError | poolsError || shipmentsError || optionsError) {
    return <div>Error loading data</div>;
  }

  const specimenPage = async (props) => {
    const barcode = props.match.params.barcode;
    const specimen = await new SpecimenAPI().getById(barcode);
    return (
      <BarcodePageProvider>
        <SpecimenPage
          key={barcode} // Ensures component resets on barcode change
          specimen={specimen}
          options={options}
        />
      </BarcodePageProvider>
    );
  };

  const containerPage = (props) => {
    return (
      <BarcodePageProvider>
        <ContainerPage
          key={props.match.params.barcode} // Ensures component resets on barcode change
          barcode={props.match.params.barcode}
        />
      </BarcodePageProvider>
    );
  };

  const tabInfo = [];                                                           
  const tabList = [];                                                           
                                                                                
  if (loris.userHasPermission('biobank_specimen_view')) {                       
    tabInfo.push({ id: 'specimens', content: <SpecimenTab/> });                    
    tabList.push({ id: 'specimens', label: 'Specimens' });                      
  }                                                                             
                                                                                
  if (loris.userHasPermission('biobank_container_view')) {                      
    tabInfo.push({ id: 'containers', content: <ContainerTab/> });                  
    tabList.push({ id: 'containers', label: 'Containers' });                    
  }                                                                             
                                                                                
  if (loris.userHasPermission('biobank_pool_view')) {                           
    tabInfo.push({ id: 'pools', content: <PoolTab/> });                            
    tabList.push({ id: 'pools', label: 'Pools' });                              
  }                                                                             
                                                                                
  // tabInfo.push({ id: 'shipments', content: <ShipmentTab/> });                      
  // tabList.push({ id: 'shipments', label: 'Shipments' });                        
                                                                                
  const tabContent = Object.keys(tabInfo).map((key) => (                        
    <TabPane key={key} TabId={tabInfo[key].id}>                                 
      {tabInfo[key].content}                                                    
    </TabPane>                                                                  
  ));                                                                           
                                                                                
  const filter = () => (
    <div id='biobank-page'>                                                     
      <Tabs tabs={tabList} defaultTab={tabList[0].id} updateURL={true}>         
        {tabContent}                                                            
      </Tabs>                                                                   
    </div>                                                                      
  );

  const data = {
    options: options,
    specimens: specimens,
    containers: containers,
    pools: pools,
    shipments: shipments,
  };

  return (
    <BiobankProvider data={data}>
      <BrowserRouter basename='/biobank'>
        <Routes>
          <Route path='/' element={filter}/>
          <Route path='/containers/:barcode' element={containerPage}/>
          <Route path='/specimens/:barcode' element={specimenPage}/>
        </Routes>
      </BrowserRouter>
    </BiobankProvider>
  );
}

window.addEventListener('load', () => {
  ReactDOM.render(<BiobankIndex/>, document.getElementById('lorisworkspace'));
});
