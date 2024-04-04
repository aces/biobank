import { useState, useEffect, ReactElement} from 'react';
import { Link } from 'react-router-dom';
import { SpecimenForm } from '../components';
import { Candidate, Pool, Session } from '../types';
import { mapFormOptions, clone } from '../utils';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext, useEditable } from '../hooks';
import Form from 'Form';
const {
  CTA,
} = Form;
declare const loris: any;

/**
 * React component for the Pool tab of the Biobank module.
 */
export function PoolTab() {
  const { pools, options } = useBiobankContext();
  const { editable, edit, clear } = useEditable();
                                                                                
  const openAliquotForm = (poolId) => {
    edit('aliquotForm');
  }

  /**
   * Map IDs in the pool columns to a string value.
   *
   * @param {string} column - the column name being mapped
   * @param {string} value - the column value being mapped
   *
   * @return {string}
   */
  function mapPoolColumns(column: string, value: string): string {
    switch (column) {
      case 'Type':
        return options.specimen.types[value].label;
      case 'Site':
        return options.centers[value];
      default:
        return value;
    }
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
    value: string | string[],
    row: any
  ): ReactElement {
    value = mapPoolColumns(column, value);
    // Attempt to find the candidate
    const candidate = Object.values(options.candidates)
    .find((cand: Candidate) => cand.pscid == row['PSCID']);

    // Check if a candidate was found before accessing the id
    const candId = candidate ? candidate.id : null;

    // If candId is defined, then the user has access to the candidate and a
    // hyperlink can be established.
    const candidatePermission = candId != null;
    switch (column) {
      case 'Pooled Specimens':
        const barcodes = value
          .map((barcode, i) => {
            return <Link key={i} to={`/specimens/${barcode}`}>{barcode}</Link>;
          })
          .reduce((prev, curr) => [prev, ', ', curr]);
        return <td>{barcodes}</td>;
      case 'PSCID':
        if (candidatePermission) {
          return <td><a href={loris.BaseURL + '/' + candId}>{value}</a></td>;
        }
        return <td>{value}</td>;
      case 'Visit Label':
        if (candidatePermission) {
          const ses = Object.values(options.candidateSessions[candId]).find(
            (sess: Session) => sess.label == value
          ).id;
          const visitLabelURL = loris.BaseURL+'/instrument_list/?candID='+candId+
            '&sessionID='+ses;
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

  const specimenTypes = mapFormOptions(
    options.specimen.types, 'label'
  );
  const poolData = Object.values(pools).map((pool: Pool) => {
    return [
      pool.id,
      pool.label,
      Math.round(pool.quantity*100)/100 +
         ' ' +
          options.specimen.units[pool.unitId].label,
      pool.specimenBarcodes,
      pool.candidatePSCID,
      options.sessions[pool.sessionId].label,
      pool.typeId,
      pool.centerId,
      pool.date,
      pool.time,
    ];
  });

  const fields = [
    {label: 'ID', show: false},
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
      options: specimenTypes,
    }},
    {label: 'Site', show: true, filter: {
      name: 'site',
      type: 'select',
      options: options.centers,
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
        getMappedCell={mapPoolColumns}
      />
      {renderAliquotForm()}
    </div>
  );
}
