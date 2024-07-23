import { Fragment, useState, useEffect, ReactElement} from 'react';
import { Link } from 'react-router-dom';
import { SpecimenForm } from '../components';
import { IPool } from '../entities';
import { mapFormOptions, clone } from '../utils';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext, useEditable, useRequest } from '../hooks';
import { BaseAPI, SpecimenAPI } from '../APIs';
import Form from 'Form';
const {
  CTA,
} = Form;
declare const loris: any;

export const PoolTab: React.FC = () => {
  const { pools, poolProg: progress } = useBiobankContext();
  const { editable, edit, clear } = useEditable();
                                                                                
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
    switch (column) {
      case 'Pooled Specimens':
        const barcodesArray = typeof value === 'string' ? [value] : value;
        const barcodes = barcodesArray.map((barcode, i) => {
            <Fragment key={i}>
              {i > 0 && ', '}
              <Link to={`/specimens/${barcode}`}>{barcode}</Link>;
            </Fragment>
          })
        return <td>{barcodes}</td>;
      case 'PSCID':
        if (value.id) {
          return <td><a href={loris.BaseURL + '/' + value.id}>{value.label}</a></td>;
        }
        return <td>{value.label}</td>;
      case 'Visit Label':
        if (value.id) {
          const visitLabelURL =
            loris.BaseURL+'/instrument_list/?candID='+row['PSCID'].id+
            '&sessionID='+value.id;
          return <td><a href={visitLabelURL}>{value}</a></td>;
        }
        return <td>{value}</td>; 
      case 'Aliquot':
        const onClick = () => openAliquotForm(row['ID']);
        return <td><CTA label='Aliquot' onUserInput={onClick}/></td>;
      default:
        return <td>{value}</td>;
    }
  }

  const renderAliquotForm = () => {
    // TODO: This should be fixed. A lot of hacks are being used to initialize
    // this form and there's definitely better ways to be doing it.
    // if (!(loris.userHasPermission('biobank_specimen_create')
    //     && poolId)
    // ) {
    //   return;
    // }

    return (
      <SpecimenForm
        title='Aliquot Pool'
        show={editable.aliquotForm}
        onClose={clear}
      />
    );
  }

  const poolData = Object.values(pools).map((pool: IPool) => {
    return [
      pool.label,
      Math.round(pool.quantity*100)/100 + ' ' + pool.unit,
      pool.specimens,
      pool.candidate,
      pool.session,
      pool.type,
      pool.center,
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
      options: useRequest(new SpecimenAPI('types')),
    }},
    {label: 'Site', show: true, filter: {
      name: 'site',
      type: 'select',
      options: useRequest(new BaseAPI('centers')),
    }},
    {label: 'Date', show: true},
    {label: 'Time', show: true},
    {label: 'Aliquot', show: true},
  ];

  return (
    <div>
      <FilterableDataTable
        name='pool'
        data={poolData}
        fields={fields}
        getFormattedCell={formatPoolColumns}
        progress={progress}
      />
      {renderAliquotForm()}
    </div>
  );
}
