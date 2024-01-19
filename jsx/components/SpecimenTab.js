import { useState, useEffect } from 'react';
import { Link } from 'react-router-dom';
import { clone, mapFormOptions } from '../utils';
import FilterableDataTable from 'FilterableDataTable';
import { useBiobankContext } from '../hooks';
import {
  SpecimenForm,
  PoolSpecimenForm,
  BatchProcessForm,
  BatchEditForm,
  Search,
} from '../components';

/**
 * JSX Component representing the specimen tab of the biobank
 * module.
 */
function SpecimenTab () {
  const { options, specimens } = useBiobankContext();
  const [editable, setEditable] = useState({});

  function saveBatchEdit(list) {                                                         
    const saveList = list                                                       
    .map((specimen) => () => specimen.put());    
                                                                                
    return Promise.all(saveList.map((item) => item()))
    .then(() => Swal.fire('Batch Preparation Successful!', '', 'success'));    
  }    

  /**
   * Make the form editable
   *
   * @param {object} stateKey - the key holding the state
   *
   * @return {Promise}
   */
  function edit(stateKey) {
    const newEditable = clone(editable);
    newEditable[stateKey] = true;
    setEditable(editable);
  }

  /**
   * Clear the editable state of this tab.
   */
  function clearEditable() {
    setEditable({});
  }

  /**
   * Map a specimen id to a string value for display.
   *
   * @param {string} column - the column name being mapped
   * @param {string} value - the value being mapped
   *
   * @return {string}
   */
  function mapSpecimenColumns(column, value) {
    switch (column) {
      case 'Type':
        return options.specimen.types[value].label;
      case 'Container Type':
        return options.container.typesPrimary[value].label;
      case 'Diagnosis':
        if (value) {
          return value.map((id) => options.diagnoses[id].label);
        }
        break;
      case 'Status':
        return options.container.stati[value].label;
      case 'Current Site':
        return options.centers[value];
      case 'Draw Site':
        return options.centers[value];
      case 'Projects':
        return value.map((id) => options.projects[id]);
      default:
        return value;
    }
  }

  /**
   * Format columns for a specimen row
   *
   * @param {string} column - the column name being mapped
   * @param {string} value - the value being mapped
   * @param {array} row - an array of the row values
   *
   * @return {ReactDOM}
   */
  function formatSpecimenColumns(column, value, row) {
    value = mapSpecimenColumns(column, value);
    const candidate = Object.values(options.candidates)
      .find((cand) => cand.pscid == row['PSCID']);
    const candidatePermission = candidate !== undefined;
    switch (column) {
      case 'Barcode':
        return <td><Link to={`/specimens/${value}`}>{value}</Link></td>;
      case 'Parent Specimens':
        // TODO: if the user doesn't have access then these shouldn't be hyperlinked
        const barcodes = value && value.map((id, key) => {
          return <Link key={key} to={`/specimens/${value}`}>{value}</Link>;
        }).reduce((prev, curr) => [prev, ', ', curr]);
        return <td>{barcodes}</td>;
      case 'PSCID':
        if (candidatePermission) {
          return <td><a href={loris.BaseURL + '/' + candidate.id}>{value}</a></td>;
        }
        return <td>{value}</td>;
      case 'Visit Label':
        if (candidatePermission) {
          const ses = Object.values(options.candidateSessions[candidate.id]).find(
            (sess) => sess.label == value
          ).id;
          const visitLabelURL = loris.BaseURL+'/instrument_list/?candID='+candidate.id+
            '&sessionID='+ses;
          return <td><a href={visitLabelURL}>{value}</a></td>;
        }
        return <td>{value}</td>;
      case 'Status':
        const style = {};
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
      case 'Projects':
        return <td>{value.join(', ')}</td>;
      case 'Container Barcode':
        // TODO: this should not be a link when user doesn't have permission to view
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;
      default:
        return <td>{value}</td>;
     }
  }

  const barcodesPrimary = Object.values(specimens)
    .reduce((result, specimen) => {
      result[specimen.barcode] = specimen.barcode;
      return result;
    }, {});
  const specimenTypes = mapFormOptions(options.specimen.types, 'label');
  const containerTypesPrimary = mapFormOptions(
      options.container.typesPrimary, 'label'
  );
  const stati = mapFormOptions(options.container.stati, 'label');
  const diagnoses = mapFormOptions(options.diagnoses, 'label');
  const specimenData = Object.values(specimens).map((specimen) => {
    let specimenAttributeData = [];
    Object.keys(options.specimen.processAttributes)
      .forEach((processId) => {
        Object.keys(options.specimen.processAttributes[processId])
          .forEach((attributeId) => {
            const sopt = options.specimen;
            const process = sopt.processes[processId].label.toLowerCase();
            if ((specimen[process]||{}).data) {
              const processIdStr = specimen[process].protocolId.toString();
              const attrs = options.specimen.processAttributes;
              const protocols = attrs[processId][attributeId].protocolIds;
              if (protocols.includes(processIdStr)) {
                const data = specimen[process].data[attributeId];
                specimenAttributeData.push(data);
              } else {
                specimenAttributeData.push(null);
              }
            }
          });
      });
    const candidate = options.candidates[specimen.candidateId];
    return [
      specimen.barcode,
      specimen.typeId,
      specimen.container.typeId,
      specimen.quantity+' '+options.specimen.units[specimen.unitId].label,
      specimen.fTCycle || null,
      specimen.parentSpecimenBarcodes,
      specimen.candidatePSCID,
      candidate?.sex || null,
      specimen.candidateAge,
      candidate?.diagnosisIds || null,
      options.sessions[specimen.sessionId].label,
      specimen.poolLabel,
      specimen.container.statusId,
      specimen.projectIds,
      specimen.centerId,
      options.sessionCenters[specimen.sessionId]?.centerId,
      specimen.collection.date,
      specimen.collection.time,
      (specimen.preparation||{}).time,
      specimen.container.parentContainerBarcode,
      specimen.container.coordinate,
      ...specimenAttributeData,
    ];
  });

  let specimenAttributeFields = [];
  Object.keys(options.specimen.processAttributes)
    .forEach((processId) => {
      Object.keys(options.specimen.processAttributes[processId])
        .forEach((attributeId) => {
          specimenAttributeFields.push(
            {
              label: options.specimen.attributes[attributeId].label,
              show: true,
            },
          );
        });
    });
  const fields = [
    {label: 'Barcode', show: true, filter: {
      name: 'barcode',
      type: 'text',
    }},
    {label: 'Type', show: true, filter: {
      name: 'type',
      type: 'select',
      options: specimenTypes,
    }},
    {label: 'Container Type', show: true, filter: {
      name: 'containerType',
      type: 'select',
      options: containerTypesPrimary,
    }},
    {label: 'Quantity', show: true},
    {label: 'F/T Cycle', show: false, filter: {
      name: 'fTCycle',
      type: 'text',
      hide: true,
    }},
    {label: 'Parent Specimen(s)', show: false, filter: {
      name: 'parentSpecimens',
      type: 'text',
      hide: true,
    }},
    {label: 'PSCID', show: true, filter: {
      name: 'pscid',
      type: 'text',
    }},
    {label: 'Sex', show: true, filter: {
      name: 'sex',
      type: 'select',
      options: {Male: 'Male', Female: 'Female'},
    }},
    {label: 'Age at Collection', show: true, filter: {
      name: 'age',
      type: 'number',
    }},
    {label: 'Diagnosis', show: true, filter: {
      name: 'diagnosis',
      type: 'multiselect',
      options: diagnoses,
    }},
    {label: 'Visit Label', show: true, filter: {
      name: 'session',
      type: 'text',
    }},
    {label: 'Pool', show: false, filter: {
      name: 'pool',
      type: 'text',
      hide: true,
    }},
    {label: 'Status', show: true, filter: {
      name: 'status',
      type: 'select',
      options: stati,
    }},
    {label: 'Projects', show: true, filter: {
      name: 'projects',
      type: 'multiselect',
      options: options.projects,
    }},
    {label: 'Current Site', show: true, filter: {
      name: 'currentSite',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Draw Site', show: true, filter: {
      name: 'drawSite',
      type: 'select',
      options: options.centers,
    }},
    {label: 'Collection Date', show: true, filter: {
      name: 'collectionDate',
      type: 'date',
    }},
    {label: 'Collection Time', show: true, filter: {
      name: 'collectionTime',
      type: 'text',
    }},
    {label: 'Preparation Time', show: true, filter: {
      name: 'preparationTime',
      type: 'text',
    }},
    {label: 'Container Barcode', show: true, filter: {
      name: 'containerBarcode',
      type: 'text',
    }},
    {label: 'Coordinate', show: true},
    ...specimenAttributeFields,
  ];

  const openSearchSpecimen = () => edit('searchSpecimen');
  const openSpecimenForm = () => edit('specimenForm');
  const openPoolForm = () => edit('poolSpecimenForm');
  const openBatchProcessForm = () => edit('batchProcessForm');
  const openBatchEditForm = () => edit('batchEditForm');
  const actions = [
    {
      name: 'goToSpecimen',
      label: 'Go To Specimen',
      action: openSearchSpecimen,
    },
    {name: 'addSpecimen', label: 'Add Specimen', action: openSpecimenForm},
    {name: 'poolSpecimen', label: 'Pool Specimens', action: openPoolForm},
    {
      name: 'batchProcess',
      label: 'Process Specimens',
      action: openBatchProcessForm,
    },
    {name: 'batchEdit', label: 'Edit Specimens', action: openBatchEditForm},
  ];

  return (
    <div>
      <FilterableDataTable
        name='specimen'
        data={specimenData}
        fields={fields}
        actions={actions}
        getFormattedCell={formatSpecimenColumns}
        getMappedCell={mapSpecimenColumns}
      />
      <Search
        title='Go To Specimen'
        show={editable.searchSpecimen}
        onClose={clearEditable}
        barcodes={barcodesPrimary}
      />
      {loris.userHasPermission('biobank_specimen_create') ?
      <SpecimenForm
        title='Add New Specimen'
        options={options}
        show={editable.specimenForm}
        onClose={clearEditable}
      /> : null}
      {loris.userHasPermission('biobank_pool_create') ?
      <PoolSpecimenForm
        options={options}
        show={editable.poolSpecimenForm}
        onClose={clearEditable}
      /> : null}
      {/*
        {loris.userHasPermission('biobank_specimen_update') &&
          <BatchProcessForm
            show={editable.batchProcessForm}
            onClose={clearEditable}
            options={options}
          />
        }
        {loris.userHasPermission('biobank_specimen_update') &&
          <BatchEditForm
            show={editable.batchEditForm}
            onClose={clearEditable}
            onSubmit={saveBatchEdit}
            options={options}
          />
        }
      */}
    </div>
  );
}

export default SpecimenTab;
