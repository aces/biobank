import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext } from '../hooks';
// import Container from './Container';
import { IShipment, Shipment, ILog, Log, useShipment, useLog } from '../entities';
import { ShipmentField, LogField } from './';
import TriggerableModal from 'TriggerableModal';
import { ShipmentAPI } from '../APIs';
import dict from '../i18n/en.json';
import Form from 'Form';
const {
  InputList,
  SelectElement,
  StaticElement,
  TextboxElement,
  TextareaElement,
  HorizontalRule,
  DateElement,
  TimeElement,
} = Form;

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

  const { options, shipments, shipProg: progress} = useBiobankContext();

  // TODO it will generally be good to have the context objects be updatable.
  // so this should be implemented again
  // const updateShipments = (updatedShipments: IShipment[]) => {
  //   updatedShipments.forEach((shipment) => {
      // setShipments({
      //   ...shipments,
      //   [shipment.barcode]: shipment,
      // });
  //  });
  // };

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
          return (
            <td>
              <ReceiveShipment
                shipment={shipments[row['Barcode']]}
              />
            </td>
          );
        }
      default:
        return <td>{value}</td>;
    }
  };

  const shipmentData = Object.values(shipments).map((shipment: IShipment) => {
    return [
      shipment.barcode,
      shipment.type,
      shipment.status,
      shipment.originCenter,
      shipment.destinationCenter,
    ];
  });

  const fields = [
    {label: 'ID', show: false},
    {label: 'Barcode', show: true, filter: {
      name: 'barcode',
      type: 'text',
    }},
    {label: 'Type', show: true, filter: {
      name: 'type',
      type: 'select',
      options: options.shipment.types,
    }},
    {label: 'Status', show: true, filter: {
      name: 'status',
      type: 'select',
      options: options.shipment.statuses,
    }},
    {label: 'Origin Center', show: true, filter: {
      name: 'originCenter',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Destination Center', show: true, filter: {
      name: 'destinationCenter',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Actions', show: true},
  ];

  const forms = [<CreateShipment/>];

  return (
    <FilterableDataTable
      progress={progress}
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
  const logs = shipment.logs.map((log, i) => {
    return (
    <>
      <h4>Shipment Log {i+1}</h4>
      <HorizontalRule/>
      <StaticElement label='Center' text={log.center}/>
      <StaticElement label='Status' text={log.status}/>
      <StaticElement label='Temperature' text={log.temperature}/>
      <StaticElement label='Date' text={log.date}/>
      <StaticElement label='Time' text={log.time}/>
      <StaticElement label='User' text={log.user}/>
      <StaticElement label='Comments' text={log.comments}/>
    </>
    );
  });

  const containers = shipment.containers.map((barcode, i) => (
    <React.Fragment key={i}>
      {i > 0 && ', '}
      <Link to={'/containers/'+barcode}>
        {barcode}
      </Link>
    </React.Fragment>
  ))

  return (
    <>
      <StaticElement label='Barcode' text={shipment.barcode}/>
      <StaticElement label='Type' text={shipment.type}/>
      <StaticElement label='Containers' text={containers}/>
      <StaticElement label='Origin Center' text={shipment.logs[0].center}/>
      <StaticElement label='Destination Center' text={shipment.destinationCenter}/>
      {logs}
    </>
  );
}

// TODO: this will be reinstated as a prop when I start re-instating context updates
// updateShipments: (updatedShipments: Shipment[]) => void,
const CreateShipment: React.FC = () => {

  const { containers } = useBiobankContext();

  const logIndex = 0;
  const shipment = useShipment({logs: [{status: 'created'}]});
  const log = shipment.logs[logIndex];

  const setLog = (log) => shipment.setLog(logIndex, log);

  const onSubmit = async () => {
    const container = containers(shipment.containers[0]);
    const l = new Log(log).set('center', container.center);
    shipment.setLog(logIndex, l.getData());
    // const entities = await shipment.post();
    //updateShipments(entities.shipments);
  };

  return (
    <TriggerableModal
      label='Create Shipment'
      title='Create Shipment'
      onSubmit={onSubmit}
      onClose={shipment.clear}
    >
      <StaticElement label='Note' text={dict.noteForCreateShipment}/>
      <ShipmentField property='barcode' shipment={shipment}/>
      <ShipmentField property='type' shipment={shipment}/>
      <InputList
        name='containers'
        label="Container"
        items={shipment.containers}
        setItems={shipment.set}
        options={{}} //TODO: fill this with call to get all containers
        errorMessage={shipment.errors.containers}
      />
      <ShipmentField property='destinationCenter' shipment={shipment}/>
      <ShipmentLogForm log={log} setLog={setLog}/>
    </TriggerableModal>
  );
}

/**
 * React Component for a received shipment
 */
// TODO: this will be reinstated as a prop when I start re-instating context updates
// updateShipments: (updatedShipments: Shipment[]) => void,
const ReceiveShipment: React.FC<{
  shipment: IShipment,
}> = ({
  shipment: initShipment,
}) => {
  const shipment = useShipment(initShipment);
  const logIndex = shipment.logs.length-1;
  const onSuccess = ({shipments, containers}) => {
    // updateShipments(shipments);
  };

  const onOpen = () => {
    shipment.addLog({status: 'received', center: shipment.destinationCenter});
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
    <>
      <LogField property={'temperature'} log={logHook}/>
      <LogField property={'date'} log={logHook}/>
      <LogField property={'time'} log={logHook}/>
      <LogField property={'user'} log={logHook}/>
      <LogField property={'comments'} log={logHook}/>
    </>
  );
}

export default ShipmentTab;
