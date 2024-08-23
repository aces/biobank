import ReactDOM from 'react-dom';
import React from 'react';
import { QueryClient, QueryClientProvider } from 'react-query';
import { BrowserRouter, Switch, Route } from 'react-router-dom';
import { Tabs, TabPane } from 'Tabs';
import {
  ContainerPage,
  SpecimenPage,
  SpecimenTab,
  ContainerTab,
  PoolTab,
  ShipmentTab,
} from '../components';
import { useBiobankContext } from '../hooks';
import { BiobankProvider, BarcodePageProvider } from '../contexts';

declare const loris: any;

/**
 * The main React entrypoint for the biobank module. This component
 * renders the index page.
 */
const BiobankIndex = () => {
  const tabInfo = [];
  const tabList = [];

  if (loris.userHasPermission('biobank_specimen_view')) {
    tabInfo.push({ id: 'specimens', content: <SpecimenTab /> });
    tabList.push({ id: 'specimens', label: 'Specimens' });
  }

  if (loris.userHasPermission('biobank_container_view')) {
    tabInfo.push({ id: 'containers', content: <ContainerTab /> });
    tabList.push({ id: 'containers', label: 'Containers' });
  }

  if (loris.userHasPermission('biobank_pool_view')) {
    tabInfo.push({ id: 'pools', content: <PoolTab /> });
    tabList.push({ id: 'pools', label: 'Pools' });
  }

  tabInfo.push({ id: 'shipments', content: <ShipmentTab /> });
  tabList.push({ id: 'shipments', label: 'Shipments' });

  // Updated Tab Content: Wrapped in LazyTabContent for lazy loading
  const tabContent = tabInfo.map((tab) => (
    <TabPane key={tab.id} TabId={tab.id}>
      {tab.content}
    </TabPane>
  ));

  const fallback = <div>Loading...</div>;

  const filter = () => (
    <div id='biobank-page'>
      <Tabs tabs={tabList} defaultTab={tabList[0].id} updateURL={true}>
        {tabContent}
      </Tabs>
    </div>
  );

  return (
    <BrowserRouter basename='/biobank'>
      <Switch>
        <Route path='/' render={filter} />
        <Route path='/containers/:barcode' render={() => <ContainerPage />} />
        <Route path='/specimens/:barcode' render={() => <SpecimenPage />} />
      </Switch>
    </BrowserRouter>
  );
};

const queryClient = new QueryClient();
window.addEventListener('load', () => {
  ReactDOM.render(
    <QueryClientProvider client={queryClient}>
      <BiobankProvider>
        <BiobankIndex />
      </BiobankProvider>
    </QueryClientProvider>,
    document.getElementById('lorisworkspace')
  );
});

