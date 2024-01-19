import { BrowserRouter, Route, Switch } from 'react-router-dom';
import { useState, useEffect } from 'react';
import { Tabs, TabPane } from 'Tabs';
import {
  SpecimenTab, 
  ContainerTab, 
  PoolTab, 
  ShipmentTab, 
  SpecimenPage,
  ContainerPage 
} from '../components';
import { OptionAPI, SpecimenAPI, ContainerAPI, PoolAPI, ShipmentAPI} from '../APIs';
import { BiobankProvider, BarcodePageProvider } from '../contexts';

/**
 * The main React entrypoint for the biobank module. This component
 * renders the index page.
 */
function BiobankIndex() {
  const { options, isLoading: optWait, error } = useHTTPRequest( () => 
    OptionAPI.getAll()
  );
  const { containers, isLoading: contWait } = useHTTPRequest( () => ContainerAPI.getAll());
  const { specimens, isLoading: specWait } = useHTTPRequest( () => SpecimenAPI.getAll());
  const { pools, isLoading: poolWait } = useHTTPRequest( () => PoolAPI.getAll());
  const { shipments, isLoading: shipWait } = useHTTPRequest( () => ShipmentAPI.getAll());

  const specimenPage = async (props) => {
    const barcode = props.match.params.barcode;
    const specimen = await SpecimenAPI.getById(barcode);
    return (
      <BarcodePageProvider>
        <SpecimenPage
          key={barcode}
          history={props.history}
          specimen={specimen}
          container={specimen.container}
          options={options}
        />
      </BarcodePageProvider>
    );
  };

  const containerPage = async (props) => {
    const barcode = props.match.params.barcode;
    const container = await ContainerAPI.getById(barcode);
    // Render the component with the resolved container data
    return (
      <BarcodePageProvider>
        <ContainerPage
          key={barcode}
          history={props.history}
          container={container}
          options={options}
        />
      </BarcodePageProvider>
    );
  };

  const specimenTab = <SpecimenTab/>;
  const containerTab = <ContainerTab/>;
  const poolTab = <PoolTab/>;
  const shipmentTab = <ShipmentTab/>;              
                                                                                
  const tabInfo = [];                                                           
  const tabList = [];                                                           
                                                                                
  if (loris.userHasPermission('biobank_specimen_view')) {                       
    tabInfo.push({ id: 'specimens', content: specimenTab });                    
    tabList.push({ id: 'specimens', label: 'Specimens' });                      
  }                                                                             
                                                                                
  if (loris.userHasPermission('biobank_container_view')) {                      
    tabInfo.push({ id: 'containers', content: containerTab });                  
    tabList.push({ id: 'containers', label: 'Containers' });                    
  }                                                                             
                                                                                
  if (loris.userHasPermission('biobank_pool_view')) {                           
    tabInfo.push({ id: 'pools', content: poolTab });                            
    tabList.push({ id: 'pools', label: 'Pools' });                              
  }                                                                             
                                                                                
  tabInfo.push({ id: 'shipments', content: shipmentTab });                      
  tabList.push({ id: 'shipments', label: 'Shipments' });                        
                                                                                
  const tabContent = Object.keys(tabInfo).map((key) => (                        
    <TabPane key={key} TabId={tabInfo[key].id}>                                 
      {tabInfo[key].content}                                                    
    </TabPane>                                                                  
  ));                                                                           
                                                                                
  const filter = (props) => (
    <div id='biobank-page'>                                                     
      <Tabs tabs={tabList} defaultTab={tabList[0].id} updateURL={true}>         
        {tabContent}                                                            
      </Tabs>                                                                   
    </div>                                                                      
  );

  if (contWait || specWait || poolWait || shipWait || optWait) {
    return <div>Loading...</div>; // Render a loader
  }

  const biobankData = {
    options: options,
    specimens: specimens,
    container: containers,
    pools: pools,
    shipments: shipments,
  };

  return (
    <BiobankProvider value={biobankData}>
      <BrowserRouter basename='/biobank'>
        <div>
          <Switch>
            <Route exact path='/' render={filter}/>
            <Route exact path='/containers/:barcode' render={containerPage}/>
            <Route exact path='/specimens/:barcode' render={specimenPage}/>
          </Switch>
        </div>
      </BrowserRouter>
    </BiobankProvider>
  );
}

window.addEventListener('load', () => {
  ReactDOM.render(<BiobankIndex/>, document.getElementById('lorisworkspace'));
});
