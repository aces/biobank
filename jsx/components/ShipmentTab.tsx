import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext, useRequest } from '../hooks';
import Utils from '../utils';
// import Container from './Container';
import { IShipment, Shipment, ILog, Log, useShipment, useLog, ShipmentProvider,
  LogProvider } from '../entities';
import { EntityType } from '../contexts';
import TriggerableModal from 'TriggerableModal';
import { BaseAPI, ShipmentAPI } from '../APIs';
import dict from '../i18n/en.json';
import { StaticField, ShipmentField, LogField} from '../forms';
import { HorizontalRule } from '../styles/form'; 

// TODO:
// - Make sure all subcontainers are loaded into shipment
// - Make sure all containers change center when shipment is received
// - Make sure to block all

/**
 * Returns a component for the shipment tab of the module
 *
 * @return {ReactDOM}
 */
const ShipmentTab: React.FC = () => {

  const { shipments, updateEntity, initializeEntity } = useBiobankContext();

  useEffect(() => {
    initializeEntity(EntityType.Shipments);
  });

  const updateShipments = (updatedShipments: IShipment[]) => {
    updatedShipments.forEach((shipment) => {
      updateEntity('Shipments', shipment.barcode, shipment);
   });
  };

  const formatShipmentColumns = (
    column: string,
    value: string,
    row: Record<string, string>
  ) => {
    switch (column) {
      case 'Barcode':
        return (
          <td>
            <TriggerableModal label={value} title={value+' Information'}>
              <ShipmentInformation shipment={shipments[value]}/>
            </TriggerableModal>
          </td>
        );
      case 'Actions':
        if (row['Status'] !== 'received') {
          // this should maybe just pass the barcode to receive shipment and
          // then upon receiving a barcode the receiveshipment component actually
          // renders itself.
          const shipment = shipments.find(shipment => shipment.barcode ===
                                          row['Barcode']);
          return <td><ReceiveShipment shipment={shipment}/></td>;
        }
      default:
        return <td>{value}</td>;
    }
  };

  const shipmentData = Object.values(shipments.data).map((shipment: IShipment) => {
    return [
      shipment.barcode,
      shipment.type,
      shipment.status,
      shipment.originCenter,
      shipment.destinationCenter,
    ];
  });

  const shipmentAPI = new ShipmentAPI();
  const centers = Utils.mapLabel(useRequest(() => new BaseAPI('centers').get()));

  const fields = [
    {label: 'ID', show: false},
    {label: 'Barcode', show: true, filter: {
      name: 'barcode',
      type: 'text',
    }},
    {label: 'Type', show: true, filter: {
      name: 'type',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => shipmentAPI.getTypes()))
    }},
    {label: 'Status', show: true, filter: {
      name: 'status',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => shipmentAPI.getStatuses()))
    }},
    {label: 'Origin Center', show: true, filter: {
      name: 'originCenter',
      type: 'select',
      options: centers,
    }},
    {label: 'Destination Center', show: true, filter: {
      name: 'destinationCenter',
      type: 'select',
      options: centers,
    }},
    {label: 'Actions', show: true},
  ];

  const forms = [<CreateShipment/>];

  return (
    <FilterableDataTable
      progress={shipments.progress}
      data={shipmentData}
      fields={fields}
      forms={forms}
      getFormattedCell={formatShipmentColumns}
    />
  );
}

const ShipmentInformation: React.FC<{
  shipment: IShipment,
}> = ({
  shipment,
}) => {

  const shipmentHook = useShipment(shipment);

  const logs = shipment.logs.map((log, i) => (
    <LogEntry key={i} log={log} index={i} />
  ));

  const containers = shipment.containers.map((barcode, i) => (
    <React.Fragment key={i}>
      {i > 0 && ', '}
      <Link to={'/containers/'+barcode}>
        {barcode}
      </Link>
    </React.Fragment>
  ))

  return (
    <ShipmentProvider shipment={shipmentHook}>
      <ShipmentField isStatic property='barcode'/>
      <ShipmentField isStatic property='type'/>
      <ShipmentField isStatic property='containers'/>
      <ShipmentField isStatic property='originCenter'/>
      <ShipmentField isStatic property='destinationCenter'/>
      {logs}
    </ShipmentProvider>
  );
}

const LogEntry: React.FC<{ log: Partial<ILog>; index: number }> = ({ log, index }) => {
  const logHook = useLog(log); // Assuming useLog is your custom hook

  return (
    <LogProvider log={logHook}>
      <h4>Shipment Log {index + 1}</h4>
      <HorizontalRule />
      <LogField isStatic property='center'/>
      <LogField isStatic property='status'/>
      <LogField isStatic property='temperature'/>
      <LogField isStatic property='date'/>
      <LogField isStatic property='time'/>
      <LogField isStatic property='user'/>
      <LogField isStatic property='comments'/>
    </LogProvider>
  );
};

// TODO: this will be reinstated as a prop when I start re-instating context updates
// updateShipments: (updatedShipments: Shipment[]) => void,
const CreateShipment: React.FC = () => {

  const { containers, updateEntities } = useBiobankContext();

  const logIndex = 0;
  const shipment = useShipment({logs: [{status: {label: 'created'}}]});
  const log = shipment.logs[logIndex];

  const setLog = (log) => shipment.setLog(logIndex, log);

  const onSubmit = async () => {
    const container = containers(shipment.containers[0]);
    const l = new Log(log).set('center', container.center);
    shipment.setLog(logIndex, l.getData());
    const entities = await ShipmentAPI.create(shipment);
    updateEntities('shipments', entities.shipments);
  };

  return (
    <TriggerableModal
      label='Create Shipment'
      title='Create Shipment'
      onSubmit={onSubmit}
      onClose={shipment.clear}
    >
      <ShipmentProvider shipment={shipment}>
        <StaticField label='Note' value={dict.noteForCreateShipment}/>
        <ShipmentField property='barcode'/>
        <ShipmentField property='type'/>
        <ShipmentField property='containers'/>
        <ShipmentField property='destinationCenter'/>
        <ShipmentLogForm log={log} setLog={setLog}/>
      </ShipmentProvider>
    </TriggerableModal>
  );
}

/**
 * React Component for a received shipment
 */
// TODO: this will be reinstated as a prop when I start re-instating context updates
// updateShipments: (updatedShipments: Shipment[]) => void,
const ReceiveShipment = ({
  shipment: initShipment
}: { shipment: IShipment }) => {
  const shipment = useShipment(initShipment);
  const logIndex = shipment.logs.length-1;
  const onSuccess = ({shipments, containers}) => {
    // updateShipments(shipments);
  };

  const onOpen = () => {
    shipment.addLog({status: {label: 'received'}, center: shipment.destinationCenter});
  };

  return (
    <TriggerableModal
      label='Receive Shipment'
      title={'Receive Shipment '+shipment.barcode}
      onUserInput={onOpen}
      onSubmit={() => ShipmentAPI.create(shipment)}
      onSuccess={onSuccess}
      onClose={shipment.clear}
    >
      <ShipmentLogForm
        log={shipment.logs[logIndex]}
        setLog={(log) => shipment.setLog(logIndex, log)}
      />
    </TriggerableModal>
  );
}

/**
 * Return a form for the shipment log
 *
 * @param {object} log - the log
 * @param {callback} setLog - a callback for when the log is set
 *
 * @return {ReactDOM[]}
 */
const ShipmentLogForm: React.FC<{
  log: Partial<ILog>,
  setLog: (log: Partial<ILog>) => void,
}> = ({ log, setLog }) => {

  const logHook = useLog(log);

  useEffect(() => {
    setLog(logHook.getData());
  }, [log])

  return (
    <LogProvider log={logHook}>
      <LogField property={'temperature'}/>
      <LogField property={'date'}/>
      <LogField property={'time'}/>
      <LogField property={'user'}/>
      <LogField property={'comments'}/>
    </LogProvider>
  );
}

export default ShipmentTab;
