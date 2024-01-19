import { useState, useEffect } from 'react';
import { Link} from 'react-router-dom';
import { Search, ContainerForm } from '../components';
import FilterableDataTable from 'FilterableDataTable';
import { mapFormOptions, clone} from '../utils';
import { useBarcodeContext } from '../hooks';
import { ContainerAPI } from '../APIs';

/**
 * React component for the Container tab in the Biobank module
 */
function ContainerTab() {

  const { options, containers } = useBarcodeContext();
  const [editable, setEditable] = useState({});                                 

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
   * Map the columns for this container
   *
   * @param {string} column - the column name
   * @param {string} value - the column value
   *
   * @return {string}
   */
  function mapContainerColumns(column, value) {
    switch (column) {
      case 'Type':
        return options.container.types[value].label;
      case 'Status':
        return options.container.stati[value].label;
      case 'Site':
        return options.centers[value];
      default:
        return value;
    }
  }

  /**
   * Format the cells for a column in the container
   *
   * @param {string} column - the column name to format
   * @param {string} value - the value of the column
   * @param {object} row - the rest of the row
   *
   * @return {JSX} a table cell
   */
  function formatContainerColumns(column, value, row) {
    value = mapContainerColumns(column, value);
    switch (column) {
      case 'Barcode':
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;
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
      case 'Parent Barcode':
        return <td><Link to={`/containers/${value}`}>{value}</Link></td>;
      default:
        return <td>{value}</td>;
    }
  }

  const stati = mapFormOptions(
    options.container.stati, 'label'
  );
  const containerTypesNonPrimary = mapFormOptions(
    options.container.typesNonPrimary, 'label'
  );

  // TODO: DELETE THIS!!!!
  // const containersNonPrimary = Object.values(containers)
  //   .reduce((result, container) => {
  //     // TODO: this check is necessary or else the page will go blank when the
  //     // first specimen is added.
  //     if (container) {
  //       const tprops = options.container.types;
  //       if (tprops[container.typeId].primary == 0) {
  //         result[container.id] = container;
  //       }
  //       return result;
  //     }
  //   }, {});

  const barcodesNonPrimary = mapFormOptions(
    containers, 'barcode'
  );

  const data = Object.values(containers).map(
    (container) => {
      return [
        container.barcode,
        container.typeId,
        container.statusId,
        container.centerId,
        container.parentContainerId ?
          containers[container.parentContainerId].barcode :
          null,
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
      options: containerTypesNonPrimary,
    }},
    {label: 'Status', show: true, filter: {
      name: 'status',
      type: 'select',
      options: stati,
    }},
    {label: 'Site', show: true, filter: {
      name: 'currentSite',
      type: 'select',
      options: options.centers,
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
    <div>
      <FilterableDataTable
        name='container'
        data={data}
        fields={fields}
        actions={actions}
        getFormattedCell={formatContainerColumns}
        getMappedCell={mapContainerColumns}
      />
      <Search
        title='Go To Container'
        show={editable.searchContainer}
        onClose={clearEditable}
        barcodes={barcodesNonPrimary}
      />
      {loris.userHasPermission('biobank_container_create') ?
      <ContainerForm
        options={options}
        show={editable.containerForm}
        onClose={clearEditable}
      /> : null}
    </div>
  );
  
}

export default ContainerTab;
