import { useState, useEffect, ReactElement, CSSProperties } from 'react';
import { Link } from 'react-router-dom';
import { ContainerForm } from '../forms';
import { Search } from '../components';
import FilterableDataTable from 'FilterableDataTable';
import Utils from '../utils'
import { useBiobankContext, useEditable, useRequest } from '../hooks';
import { IContainer } from '../entities';
import { EntityType } from '../contexts';
import { ContainerAPI, BaseAPI } from '../APIs';
declare const loris: any;

export const ContainerTab: React.FC = () => {

  const { containers, initializeEntity } = useBiobankContext();
  const { editable, edit, clear } = useEditable();

  useEffect(() => {
    initializeEntity(EntityType.Containers);
  });

  /**
   * Format the cells for a column in the container
   *
   * @param {string} column - the column name to format
   * @param {string} value - the value of the column
   * @param {object} row - the rest of the row
   *
   * @return {ReactElement} a table cell
   */
  function formatContainerColumns(column: string, value: any, row: any): ReactElement {

    const style: CSSProperties = {};

    switch (column) {
      case 'Barcode':
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;

      // case 'Status':
      //   switch (value) {
      //     case 'Available':
      //       style.color = 'green';
      //       break;
      //     case 'Reserved':
      //       style.color = 'orange';
      //       break;
      //     case 'Dispensed':
      //       style.color = 'red';
      //       break;
      //     case 'Discarded':
      //       style.color = 'red';
      //       break;
      //   }
      //   return <td style={style}>{value}</td>;

      case 'Parent Barcode':
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;

      default:
        return <td>{value}</td>;
    }
  }

  const data = containers.data.map((container: IContainer) => {
      return [
        container.barcode,
        container.type.label,
        container.center.label,
        container.parent?.barcode,
      ];
    }
  );

  const fields = [
    {label: 'Barcode', show: true, filter: {
      name: 'barcode',
      type: 'text',
    }},
    {label: 'Type', show: true, filter: {
      name: 'type',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => new ContainerAPI().getTypes()))
    }},
    {label: 'Site', show: true, filter: {
      name: 'currentSite',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => new BaseAPI('centers').get()))
    }},
    {label: 'Parent Barcode', show: true, filter: {
      name: 'parentBarcode',
      type: 'text',
    }},
  ];

  const actions = [
    {
      name: 'goToContainer',
      label: 'Go To Container',
      action: () => edit('searchContainer'),
    },
    {
      name: 'addContainer',
      label: 'Add Container',
      action: () => edit('containerForm'),
    },
  ];

  return (
    <>
      <FilterableDataTable
        name='container'
        data={data}
        fields={fields}
        actions={actions}
        getFormattedCell={formatContainerColumns}
        progress={containers.progress}
      />
      <Search
        title='Go To Container'
        show={editable.searchContainer}
        onClose={clear}
        barcodes={containers.data}
      />
      {loris.userHasPermission('biobank_container_create') ?
      <ContainerForm
        show={editable.containerForm}
        onClose={clear}
      /> : null}
    </>
  );
}
