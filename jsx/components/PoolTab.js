import { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import { SpecimenForm } from '../components';
import { mapFormOptions, clone } from '../utils';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext } from '../hooks';

/**
 * React component for the Pool tab of the Biobank module.
 */
function PoolTab() {
  const { pools, options } = useBiobankContext;
  const [editable, setEditable] = useState({});
  const [poolId, setPoolId] = useState(null);
                                                                                
  const edit = (stateKey) => {
    const newEditable = clone(editable);
    newEditable[stateKey] = true;
    setEditable(newEditable);
  }

  const clearEditable = () => {
    setEditable({});
  }

  const openAliquotForm = (poolId) => {
    setPoolId(poolId);
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
  const mapPoolColumns = (column, value) => {
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
  const formatPoolColumns = (column, value, row) => {
    value = mapPoolColumns(column, value);
    const candId = Object.values(options.candidates)
      .find((cand) => cand.pscid == row['PSCID']).id;

    // If candId is defined, then the user has access to the candidate and a
    // hyperlink can be established.
    const candidatePermission = candId !== undefined;
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
            (sess) => sess.label == value
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
    if (!(loris.userHasPermission('biobank_specimen_create')
        && poolId)
    ) {
      return;
    }

    return (
      <SpecimenForm
        title='Aliquot Pool'
        options={options}
        show={editable.aliquotForm}
        onClose={clearEditable}
      />
    );
  }

  const specimenTypes = mapFormOptions(
    options.specimen.types, 'label'
  );
  const poolData = Object.values(pools).map((pool) => {
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

export default PoolTab;
