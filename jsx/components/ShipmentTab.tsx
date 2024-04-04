import React, { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import FilterableDataTable from 'FilterableDataTable';
import { useShipment, useBiobankContext } from '../hooks';
// import Container from './Container';
import { Shipment, Log, Center, User } from '../types';
import TriggerableModal from 'TriggerableModal';
import { ShipmentAPI } from '../APIs';
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
export const ShipmentTab: React.FC = () => {

  const { options, shipments } = useBiobankContext();
  const users = Object.fromEntries(Object.values(options.users)
                                   .map((user: User) => [user.label, user.label]));

  // TODO it will generally be good to have the context objects be updatable.
  // so this should be implemented again
  const updateShipments = (updatedShipments: Shipment[]) => {
    updatedShipments.forEach((shipment) => {
      // setShipments({
      //   ...shipments,
      //   [shipment.barcode]: shipment,
      // });
    });
  };

  const mapShipmentColumns = (
    column: string,
    value: string,
  ): string => {
    switch (column) {
      case 'Origin Center':
        return options.centers[value];
      case 'Destination Center':
        return options.centers[value];
      default:
        return value;
    }
  };

  const formatShipmentColumns = (
    column: string,
    value: string,
    row: Record<string, string>
  ) => {
    value = mapShipmentColumns(column, value);
    switch (column) {
      case 'Barcode':
        return (
          <td>
            <TriggerableModal
              label={value}
              title={value+' Information'}
            >
              <ShipmentInformation
                shipment={shipments[value]}
                centers={options.centers}
              />
            </TriggerableModal>
          </td>
        );
      case 'Actions':
        if (row['Status'] !== 'received') {
          return (
            <td>
              <ReceiveShipment
                shipment={shipments[row['Barcode']]}
                users={users}
                updateShipments={updateShipments}
              />
            </td>
          );
        }
      default:
        return <td>{value}</td>;
    }
  };

  const shipmentData = Object.values(shipments).map((shipment: Shipment) => {
    return [
      shipment.id,
      shipment.barcode,
      shipment.type,
      shipment.status,
      shipment.originCenterId,
      shipment.destinationCenterId,
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
      name: 'originCenterId',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Destination Center', show: true, filter: {
      name: 'destinationCenterId',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Actions', show: true},
  ];

  const forms = [
    <CreateShipment
      centers={options.centers}
      types={options.shipment.types}
      users={users}
      updateShipments={updateShipments}
    />,
  ];

  return (
    <FilterableDataTable
      data={shipmentData}
      fields={fields}
      forms={forms}
      getMappedCell={mapShipmentColumns}
      getFormattedCell={formatShipmentColumns}
    />
  );
}

const ShipmentInformation: React.FC<{
  shipment: Shipment,
  centers: Center[],
}> = ({
  shipment,
  centers
}) => {
  const logs = shipment.logs.map((log, i) => {
    return (
    <>
      <h4>Shipment Log {i+1}</h4>
      <HorizontalRule/>
      <StaticElement
        label='Center'
        text={centers[log.centerId]}
      />
      <StaticElement
        label='Status'
        text={log.status}
      />
      <StaticElement
        label='Temperature'
        text={log.temperature}
      />
      <StaticElement
        label='Date'
        text={log.date}
      />
      <StaticElement
        label='Time'
        text={log.time}
      />
      <StaticElement
        label='User'
        text={log.user}
      />
      <StaticElement
        label='Comments'
        text={log.comments}
      />
    </>
    );
  });

  const containerBarcodes = shipment.containerBarcodes.map((barcode, i) => (
    <React.Fragment key={i}>
      {i > 0 && ', '}
      <Link to={'/containers/'+barcode}>
        {barcode}
      </Link>
    </React.Fragment>
  ))

  return (
    <>
      <StaticElement
        label='Barcode'
        text={shipment.barcode}
      />
      <StaticElement
        label='Type'
        text={shipment.type}
      />
      <StaticElement
        label='Containers'
        text={containerBarcodes}
      />
      <StaticElement
        label='Origin Center'
        text={centers[shipment.logs[0].centerId]}
      />
      <StaticElement
        label='Destination Center'
        text={centers[shipment.destinationCenterId]}
      />
      {logs}
    </>
  );
}

const CreateShipment: React.FC<{
  centers: Center[],
  types: Record<string, string>
  users: Record<string, string>,
  updateShipments: (updatedShipments: Shipment[]) => void,
}> = ({
  centers,
  types,
  users,
  updateShipments,
}) => {
  const { containers } = useBiobankContext();
  const logIndex = 0;
  const { shipment, handler } = new useShipment();
  const errors = handler.getErrors();
  const onSubmit = async () => {
    const entities = await handler.post();
    updateShipments(entities.shipments);
  };
  const onOpen = () => {
    handler.addLog({status: 'created'});
  };

  // If the associated shipments containers change, update the site of the log.
  useEffect(() => {
    if (shipment.containerIds.length === 1) {
      const container = containers(shipment.containerBarcodes[0]);
      handler.setLog('centerId', container.centerId, logIndex);
    }
  }, [shipment.containerIds]);

  return (
    <TriggerableModal
      label='Create Shipment'
      title='Create Shipment'
      onUserInput={onOpen}
      onSubmit={onSubmit}
      onClose={handler.clear}
    >
      <StaticElement
        label='Note'
        text='Any container or specimen added to this form will be
        dissassociated from its parent. Any children of the containers listed
        will also be added to the shipment.'
      />
      <TextboxElement
        name='barcode'
        label='Barcode'
        onUserInput={handler.set}
        value={shipment.barcode}
        errorMessage={errors.barcode}
        required={true}
      />
      <SelectElement
        name='type'
        label='Container Type'
        onUserInput={handler.set}
        value={shipment.type}
        options={types}
        errorMessage={errors.type}
        required={true}
      />
      <InputList
        name='barcode'
        label="Container"
        items={shipment.containerIds}
        setItems={handler.setContainerIds}
        options={{}} //TODO: fill this with call to get all containers
        errorMessage={errors.containerIds}
      />
      <SelectElement
        name='destinationCenterId'
        label='Destination Center'
        onUserInput={handler.set}
        value={shipment.destinationCenterId}
        options={centers}
        errorMessage={errors.destinationCenter}
        required={true}
      />
      <ShipmentLogForm
        log={shipment.logs[logIndex]}
        setLog={(name, value) => handler.setLog(name, value, logIndex)}
        errors={errors.logs[logIndex]}
        users={users}
      />
    </TriggerableModal>
  );
}

/**
 * React Component for a received shipment
 */
const ReceiveShipment: React.FC<{
  shipment: Shipment,
  users: Record<string, string>,
  updateShipments: (updateShipments: Shipment[]) => void,
}> = ({
  shipment: initShipment,
  users,
  updateShipments,
}) => {
  const [ shipment, handler ] = new useShipment(initShipment);
  const logIndex = handler.getShipment().logs.length-1;
  const onSuccess = ({shipments, containers}) => {
    updateShipments(shipments);
  };

  const onOpen = () => {
    handler.addLog(
      {status: 'received', centerId: shipment.destinationCenterId}
    );
  };

  // TODO: At the top of this form, it wouldn't hurt to have a ShipmentSummary
  // to display the pertinent information from the shipment!
  return (
    <TriggerableModal
      label='Receive Shipment'
      title={'Receive Shipment '+shipment.barcode}
      onUserInput={onOpen}
      onSubmit={handler.post}
      onSuccess={onSuccess}
      onClose={handler.clear}
    >
      <ShipmentLogForm
        log={handler.getShipment().logs[logIndex]}
        setLog={(name, value) => handler.setLog(name, value, logIndex)}
        errors={handler.getErrors().logs[logIndex]}
        users={users}
      />
    </TriggerableModal>
  );
}

/**
 * Return a form for the shipment log
 *
 * @param {object} log - the log
 * @param {callback} setLog - a callback for when the log is set
 * @param {object} errors - a list of errors
 * @param {object} users - a list of selectable users
 *
 * @return {ReactDOM[]}
 */
const ShipmentLogForm: React.FC<{
  log: Log,
  setLog: (name: string, value: any) => void,
  errors: Record<string, string>,
  users: Record<string, string>,
}> = ({
  log,
  setLog,
  errors = {},
  users,
}) => {
  return (
    <>
      <TextboxElement
        name='temperature'
        label='Temperature'
        onUserInput={setLog}
        value={log.temperature}
        errorMessage={errors.temperature}
        required={true}
      />
      <DateElement
        name='date'
        label='Date'
        onUserInput={setLog}
        value={log.date}
        errorMessage={errors.date}
        required={true}
      />
      <TimeElement
        name='time'
        label='Time'
        onUserInput={setLog}
        value={log.time}
        errorMessage={errors.time}
        required={true}
      />
      <SelectElement
        name='user'
        label='Done by'
        onUserInput={setLog}
        value={log.user}
        options={users}
        errorMessage={errors.user}
        required={true}
      />
      <TextareaElement
        name='comments'
        label='Comments'
        onUserInput={setLog}
        value={log.comments}
        errorMessage={errors.comments}
      />
    </>
  );
}
