import React, { useState, ReactElement, useMemo } from 'react';
import {VerticalTabs, TabPane} from 'Tabs';
import Modal from 'Modal';
import Loader from 'Loader';
import Form from 'Form';
const { StaticElement, FormElement, SearchableDropdown } = Form;
import { ProcessForm } from '../components';
import { useBiobankContext } from '../hooks';
import {mapFormOptions, clone, isEmpty} from '../utils';
import { Options, Container, Process, Specimen, Status } from '../types';
import Swal, { SweetAlertOptions, SweetAlertResult } from 'sweetalert2';

type BatchProcessFormState = {
  preparation: Process,
  list: { container: Container, specimen: Specimen }[],
  count: number,
  current: { [key: string]: any }, // TODO: type
  errors: { [key: string]: any }, // TODO: type
  loading: boolean,
  editable: { [key: string]: boolean },
  containerId: string,
};

const initialState: BatchProcessFormState = {
  preparation: {},
  list: [],
  count: 0,
  current: {},
  errors: { specimen: {preparation: {}} },
  loading: false,
  editable: { preparation: true },
  containerId: null,
};

type BatchProcessFormProps = {
  onClose: () => void,
  show: boolean,
};

/**                                                                             
 * BatchProcessForm is a component for managing the processing of specimens in batch. 
 * It allows users to input, edit, and submit data related to the specimen processing tasks.
 *                                                                              
 * @param {BatchProcessFormProps} props - The properties passed to the component.         
 * @returns {ReactElement} React element representing the header.               
 */   
function BatchProcessForm({
  onClose,
  show,
}: BatchProcessFormProps): ReactElement {

  const { options, containers, specimens, pools } = useBiobankContext();
  const [state, setState] = useState<BatchProcessFormState>(initialState);

  /**
   * Sets a process value in the component's state.
   * 
   * @param {string} name - The name of the state property to set.
   * @param {any} value - The value to set for the state property.
   * @returns {Promise<void>} A promise that resolves after the state is set.
   */
  const setProcess = (name: string, value: any): Promise<void> => {
    return new Promise((resolve) => {
      setState(prevState => ({ ...prevState, [name]: value }));
      resolve();
    });
  };

  /**
   * Adds a list item to a specific container.
   * 
   * @param {string} containerId - The ID of the container to add an item to.
   */
  const addListItem = (containerId: string) => {
    let { list, current, count } = clone(state);
  
    // Increase count
    count++;
  
    // Set Specimen and Container
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];
  
    // Set current global values
    current.typeId = specimen.typeId;
  
    // Set list values
    list[count] = { specimen, container };
  
    setState(prevState => ({
      ...prevState, 
      list, 
      current, 
      count, 
      containerId: null
    }));
  };

  /**
   * Sets the current pool to display, updating relevant state properties.
   * 
   * @param {string} name - The name to display.
   * @param {string} poolId - The ID of the pool to display.
   */
  const setPool = (name: string, poolId: string) => {
    const pool = clone(pools[poolId]);
  
    // Set loading state to true
    setState(prevState => ({ ...prevState, loading: true }));
  
    // Update the current pool ID
    setCurrent('poolId', poolId);
  
    // Process each specimenId in the pool
    pool.specimenIds.forEach(specimenId => {
      const listItemExists = Object.values(state.list)
                                   .some(item => item.specimen.id === specimenId);
      if (!listItemExists) {
        addListItem(specimens[specimenId].containerId);
      }
    });
  
    // Reset the poolId
    setCurrent('poolId', null);
  
    // Set loading state to false
    setState(prevState => ({ ...prevState, loading: false }));
  };


  /**
   * Removes an item from the list based on the provided key.
   * 
   * @param {string} key - The key of the item to remove from the list.
   */
  const removeListItem = (key: string) => {
    setState(prevState => {
      const { list, current } = clone(prevState);
  
      // Remove the item from the list
      delete list[key];
  
      // Update 'current' based on whether the list is empty
      const updatedCurrent = isEmpty(list) ? {} : current;
  
      // Return the updated state
      return {
        ...prevState,
        list,
        current: updatedCurrent,
        containerId: null
      };
    });
  };

  /**
   * Validates a container in the list.
   * 
   * @param {string} containerId - The ID of the container to validate.
   * @returns {boolean} - Returns true if validation is successful, false otherwise.
   */
  const validateListItem = (containerId: string): boolean => {
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];
  
    if (!isEmpty(state.list) && specimen.typeId !== state.current.typeId) {
      Swal.fire('Oops!', 'Specimens must be of the same Type', 'warning');
      return false;
    }
  
    return true;
  };

  /**
   * Validates a list of specimens.
   * 
   * @param {specimen: Specimen, container: Container}[] list
   * - The list of specimens to validate.
   * @returns {Promise<{specimen: Specimen, container: Container}[]>}
   * - A promise that resolves with the list if validation is successful, 
   * or rejects if not.
   */
  const validateList = (
    list: { specimen: Specimen, container: Container }[]
  ): Promise<typeof list> => {
    return new Promise((resolve, reject) => {
      const barcodes = Object.values(list)
        .filter(item => !!item.specimen.preparation)
        .map(item => item.container.barcode);
  
      if (barcodes.length > 0) {
        Swal.fire({
          title: 'Warning!',
          html: `Preparation for specimen(s) <b>${barcodes.join(', ')}</b> ` +
                `already exists. By completing this form, the previous ` +
                `preparation will be overwritten.`,
          icon: 'warning' as any,
          showCancelButton: true,
          confirmButtonText: 'Proceed'
        } as SweetAlertOptions).then((result: SweetAlertResult) => {
          // 'result as any' is a temporary bypass because typescript does not
          // recognized isConfirmed as a property of 'result'.
          if ((result as any).isConfirmed) {
            resolve(list);
          } else {
            reject();
          }
        });
      } else {
        resolve(list);
      }
    });
  };

  /**
   * Sets the current property being processed in the state.
   * 
   * @param {string} name - The name of the property to update in the current object.
   * @param {any} value - The new value to set for the property.
   */
  const setCurrent = (name: string, value: any) => {
    setState(prevState => {
      const currentClone = clone(prevState.current);
      currentClone[name] = value;
      return { ...prevState, current: currentClone };
    });
  };

  if (state.loading) {
    return <Loader/>;
  }

  const preparationForm = state.editable.preparation ? (
    <ProcessForm
      edit={true}
      errors={state.errors.specimen.preparation || {}}
      process={state.preparation}
      processStage='preparation'
      setParent={setProcess}
      typeId={state.current.typeId}
    />
  ) : null;

  // TODO: This should likely be filtered so that only pools that match the
  // proper criteria are left in the list.
  const poolsOptions = mapFormOptions(pools, 'label');
  const glyphStyle = {
    color: '#DDDDDD',
    marginLeft: 10,
    cursor: 'pointer',
  };

  const barcodeList = Object.entries(state.list)
    .map(([key, item]) => {
      const handleRemoveItem = () => removeListItem(key);
      return (
        <div key={key} className='preparation-item'>
          <div>{item.container.barcode}</div>
          <div
            className='glyphicon glyphicon-remove'
            style={glyphStyle}
            onClick={handleRemoveItem}
          />
        </div>
      );
    });

  const editForms = Object.keys(state.list).length > 1 && (
    <div className='form-top'>
      <VerticalTabs
        tabs={[{id: 'preparation', label: 'Preparation'}]}
        onTabChange={(id) => setState(prevState => ({ 
          ...prevState, 
          editable: { ...prevState.editable, [id]: true }
        }))}
        updateURL={false}
      >
        <TabPane TabId='preparation'>{preparationForm}</TabPane>
      </VerticalTabs>
    </div>
  );

  const handlePoolInput = (name, value) => {
    if (value) {
      setPool(name, value);
    }
  }

  const form = (
    <FormElement>
      <div className='row'>
        <div className='col-sm-10 col-sm-offset-1'>
          <StaticElement
            label='Processing Note'
            text="Select or Scan the specimens to be prepared. Specimens must
                  have a Status of 'Available', and share the same Type.
                  Any previous value associated with a Specimen will be
                  overwritten if one is added on this form."
          />
          <StaticElement
            label='Specimen Type'
            text={(options.specimen.types[state.current.typeId]||{}).label || '—'}
          />
          <div className='row'>
            <div className='col-xs-6'>
              <h4>Barcode Input</h4>
              <div className='form-top'/>
              <BarcodeInput
                options={options}
                list={state.list}
                containerId={state.containerId}
                validateListItem={validateListItem}
                addListItem={addListItem}
              />
              <SearchableDropdown
                name={'poolId'}
                label={'Pool'}
                onUserInput={handlePoolInput}
                options={poolsOptions}
                value={state.current.poolId}
              />
            </div>
            <div className='col-xs-6'>
              <h4>Barcode List</h4>
              <div className='form-top'/>
              <div className='preparation-list'>
                {barcodeList}
              </div>
            </div>
          </div>
          {editForms}
        </div>
      </div>
    </FormElement>
  );

  const handleClose = () => {
    setState(initialState);
    onClose();
  };

  const handleSubmit = () => {
    const prepList = Object.values(state.list).map((item) => {
      const specimen = clone(item.specimen);
      specimen.preparation = clone(state.preparation);
      specimen.preparation.centerId = item.container.centerId;
      return specimen;
    });

    // TODO: execute update of specimens here!
  };

  return (
    <Modal
      title='Process Specimens'
      show={show}
      onClose={handleClose}
      onSubmit={handleSubmit}
      throwWarning={true}
    >
      {form}
    </Modal>
  );
}

type BarcodeInputProps = {                                                  
  options: Options, // TODO: type
  list: {container: Container, specimen: Specimen}[],
  containerId?: string,
  addListItem: (containerId: string) => void,
  validateListItem: (containerId: string) => boolean,
};  

/**
 * `BarcodeInput` is a component for managing specimen barcode input in a
 * batch processing form. It provides a searchable dropdown for selecting or
 * inputting specimen barcodes, handling validation and addition of specimens
 * to the processing list.
 *                                                                              
 * @param {BarcodeInputProps} props - The properties passed to the component.         
 * @returns {ReactElement} React element representing the header.               
 */      
function BarcodeInput({
  list,
  containerId = null,
  addListItem,
  validateListItem,
}: BarcodeInputProps): ReactElement {
  const { options, containers, specimens } = useBiobankContext();

  // Create options for barcodes based on match typeId
  // TODO: this can later be replaced with a simple fetch call.
  const barcodesPrimary = useMemo(() => {
    return Object.values(containers).reduce((result: Record<string, string>, container: Container) => {
      const isPrimaryType = options.container.types[container.typeId]?.primary === 1;
      const specimen = specimens[container.specimenId];
      const isSpecimenValid = specimen?.typeId && 
        specimen.typeId === options?.specimen?.protocols[specimen.typeId]?.typeId;
      // TODO: replace with something else, not availability.
      // const availableId = Object.values(options.container.stati)
      //   .find(status => status.label === 'Available')?.id;
      const isInList = list.some(item => item.container.id === container.id);
  
      // previously this check also included availableId
      if (isPrimaryType && isSpecimenValid && !isInList) {
        result[container.id] = container.barcode;
      }
  
      return result;
    }, {});
  }, [specimens, containers, options, list]);

  /**
   * Handles input for container IDs. Validates the container before adding it to the list.
   * 
   * @param {string} name - The name of the input field (unused but kept for consistency).
   * @param {string} containerId - The ID of the container to validate and add.
   */
  const handleInput = async (name: string, containerId: string) => {
    if (containerId) {
      try {
        await validateListItem(containerId);
        addListItem(containerId);
      } catch (error) {
        // Handle error (e.g., show a message)
        console.error('Validation failed:', error);
      }
    }
  };
 
  return (
    <SearchableDropdown
      name={'containerId'}
      label={'Specimen'}
      onUserInput={handleInput}
      options={barcodesPrimary}
      value={containerId}
    />
  );
}

export default BatchProcessForm;
