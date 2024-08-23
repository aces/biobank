import { Fragment, useState, useEffect, ReactElement} from 'react';
import { Link } from '../components';
import { SpecimenForm } from '../forms';
import { IPool } from '../entities';
import Utils from '../utils';
import { Button } from '../forms';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext, useEditable, useRequest } from '../hooks';
import { EntityType } from '../contexts'
import { BaseAPI, SpecimenAPI } from '../APIs';
declare const loris: any;

export const PoolTab: React.FC = () => {
  const { pools, initializeEntity } = useBiobankContext();
  const { editable, edit, clear } = useEditable();

  useEffect(() => {
    initializeEntity(EntityType.Pools);
  });
                                                                                
  const openAliquotForm = (pool) => {
    edit('aliquotForm');
  }

  /**
   * Format a row of columns for the pooled specimen.
   *
   * @param {string} column - the column name
   * @param {string} value - the column value
   * @param {object} row - all the values from the row
   *
   * @return {JSX}
   */
  function formatPoolColumns(
    column: string,
    value: any,
    row: any
  ): ReactElement {
    console.log(value);
    const handlers = {
      'Pooled Specimens': () => (
        value.map((specimen, i) => (
          <Fragment key={i}>
            {i > 0 && ', '}
            <Link to={'/specimens/'+specimen.container.barcode}>
              {specimen.container.barcode}
            </Link>
          </Fragment>
        ))
      ),
      'PSCID': () => (
        <Link href={loris.BaseURL+'/'+value.id} condition={!!value.id}>
          {value.label}
        </Link>
      ),
      'Visit Label': () => (
        <Link
          href={loris.BaseURL+'/instrument_list/?candID='+row['PSCID'].id+'&sessionID='+value.id}
          condition={!!value.id}
        >
          {value}
        </Link>
      ),
      'Aliquot': () => (
        loris.userHasPermission('biobank_specimen_create') && row['ID'] ? (
          <Button label='Aliquot' onClick={() => openAliquotForm(row['ID'])} />
        ) : value
      ),
      'default': () => value
    };
    // Return the result wrapped in a <td> tag, falling back to 'default' if the column is not found.
    return <td>{(handlers[column] || handlers['default'])()}</td>;
  }

  console.log('pools:', pools);  // Check if `pools` is defined and what it contains
  console.log('pools.data:', pools.data);  // Check if `pools.data` is defined

  const poolData = (pools.data || {}).map((pool: IPool) => {
    return [
      pool.label,
      Math.round(pool.quantity*100)/100 + ' ' + pool.unit.label,
      pool.specimens.map((specimen) => specimen.container.barcode),
      pool.specimens[0].candidate.label,
      pool.specimens[0].session.label,
      pool.specimens[0].type.label,
      pool.specimens[0].container.center.label,
      pool.date,
      pool.time,
    ];
  });

  const fields = [
    {label: 'Label', show: true, filter: {
      name: 'barcode',
      type: 'text',
    }},
    {label: 'Quantity', show: true},
    {label: 'Pooled Specimens', show: true},
    {label: 'PSCID', show: true, filter: {
      name: 'pscid',
      type: 'text',
    }},
    {label: 'Visit Label', show: true, filter: {
      name: 'session',
      type: 'text',
    }},
    {label: 'Type', show: true, filter: {
      name: 'type',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => new SpecimenAPI().getTypes())),
    }},
    {label: 'Site', show: true, filter: {
      name: 'site',
      type: 'select',
      options: Utils.mapLabel(useRequest(() => new BaseAPI('centers').get())),
    }},
    {label: 'Date', show: true},
    {label: 'Time', show: true},
    {label: 'Aliquot', show: true},
  ];

  return (
    <>
      <FilterableDataTable
        name='pool'
        data={poolData}
        fields={fields}
        getFormattedCell={formatPoolColumns}
        progress={pools.progress}
      />
      <SpecimenForm
        title='Aliquot Pool'
        show={editable.aliquotForm}
        onClose={clear}
      />
    </>
  );
}
