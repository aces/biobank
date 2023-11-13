import React from 'react';
import { Tabs, TabPane } from 'Tabs';
import SpecimenTab from './specimenTab';
import ContainerTab from './containerTab';
import PoolTab from './poolTab';
import ShipmentTab from './shipmentTab';

/**
 * Render a filter in the biobank.
 * @component
 * @param {BiobankFilterProps} props - The props for the component.
 * @returns {JSX.Element} The rendered component.
 */
const BiobankFilter = ({
  data,
  options,
  saveBatchEdit,
  createPool,
  createSpecimens,
  updateSpecimens,
  editSpecimens,
  history,
  increaseCoordinate,
  loading,
}) => {
  const specimenTab = (
    <SpecimenTab
      data={data}
      options={options}
      saveBatchEdit={saveBatchEdit}
      createPool={createPool}
      createSpecimens={createSpecimens}
      updateSpecimens={updateSpecimens}
      editSpecimens={editSpecimens}
      history={history}
      increaseCoordinate={increaseCoordinate}
      loading={loading}
    />
  );

  const containerTab = (
    <ContainerTab
      data={data}
      options={options}
      createContainers={createContainers}
      history={history}
      loading={loading}
    />
  );

  const poolTab = (
    <PoolTab
      data={data}
      options={options}
      createSpecimens={createSpecimens}
      increaseCoordinate={increaseCoordinate}
      loading={loading}
    />
  );

  const shipmentTab = (
    <ShipmentTab data={data} setData={setData} options={options} />
  );

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

  return (
    <div id='biobank-page'>
      <Tabs tabs={tabList} defaultTab={tabList[0].id} updateURL={true}>
        {tabContent}
      </Tabs>
    </div>
  );
};

export default BiobankFilter;
