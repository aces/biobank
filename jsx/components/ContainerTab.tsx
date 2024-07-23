import { useState, useEffect, ReactElement, CSSProperties } from 'react';
import { Link} from 'react-router-dom';
import { Search, ContainerForm } from '../components';
import FilterableDataTable from 'FilterableDataTable';
import { mapFormOptions, clone} from '../utils';
import { useBiobankContext, useEditable, useRequest } from '../hooks';
import { IContainer } from '../entities';
import { ContainerAPI, BaseAPI } from '../APIs';
declare const loris: any;

export const ContainerTab: React.FC = () => {

  const { containers, contProg: progress } = useBiobankContext();
  const { editable, edit, clear } = useEditable();

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

      case 'Status':
        switch (value) {
          case 'Available':
            style.color = 'green';
            break;
          case 'Reserved':
            style.color = 'orange';
            break;
          case 'Dispensed':
            style.color = 'red';
            break;
          case 'Discarded':
            style.color = 'red';
            break;
        }
        return <td style={style}>{value}</td>;

      case 'Parent Barcode':
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;

      default:
        return <td>{value}</td>;
    }
  }

  const data = Object.values(containers).map(
    (container: IContainer) => {
      return [
        container.barcode,
        container.type,
        container.status,
        container.center,
        container.parent,
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
      options: useRequest(new ContainerAPI('types?field=label&primary=0')),
    }},
    {label: 'Status', show: true, filter: {
      name: 'status',
      type: 'select',
      options: useRequest(new ContainerAPI('status?field=label')),
    }},
    {label: 'Site', show: true, filter: {
      name: 'currentSite',
      type: 'select',
      options: useRequest(new BaseAPI('centers?field=label'))
    }},
    {label: 'Parent Barcode', show: true, filter: {
      name: 'parentBarcode',
      type: 'text',
    }},
  ];

  const openSearchContainer = () => edit('searchContainer');
  const openContainerForm = () => edit('containerForm');
  const actions = [
    {
      name: 'goToContainer',
      label: 'Go To Container',
      action: openSearchContainer,
    },
    {
      name: 'addContainer',
      label: 'Add Container',
      action: openContainerForm,
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
        progress={progress}
      />
      <Search
        title='Go To Container'
        show={editable.searchContainer}
        onClose={clear}
        barcodes={containers}
      />
      {loris.userHasPermission('biobank_container_create') ?
      <ContainerForm
        show={editable.containerForm}
        onClose={clear}
      /> : null}
    </>
  );
}
