import Modal from 'Modal';
import React, { useState, ReactElement} from 'react';
import { mapFormOptions, clone, isEmpty } from '../utils';
import { useBiobankContext } from '../hooks';
import { Container } from '../types';
import Swal from 'sweetalert2';
import Form from 'Form';
const {
  FormElement,
  SelectElement,
  StaticElement,
  TextboxElement,
  TimeElement,
  DateElement,
  SearchableDropdown,
} = Form;


interface Props {
  onClose: () => void,
  show: boolean,
}

/**
 * React component with form for entering pool specimens
 */
const PoolSpecimenForm: React.FC<Props> = ({
  onClose,
  show,
}) => {
  const { pool, poolHandler } = usePool();
  const { list, setList } = useState({});
  const { count, setCount } = useState<number>(0);
  const { current, setCurrent } = useState<Record<string, string>>({});
  const { errors, setError } = useState<Record<string, any>>({});
  const [containerId, setContainerId] = useState<string | undefined>();

  const { options, containers, specimens } = useBiobankContext();

  /**
   * Sets the current pool list being displayed?
   *
   * @param {int} containerId - container with pool?
   */
  const setPoolList = (containerId: string) => {
    //increase count
    const newCount = count + 1;

    // set specimen and container
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];
    
    // set current global values
    const newCurrent = isEmpty(list) ? {
      candidateId: specimen.candidateId,
      sessionId: specimen.sessionId,
      typeId: specimen.typeId,
      centerId: container.centerId,
    } : current;

    // set list values
    const newList = { ...list, [newCount]: { container, specimen } };

    // set pool values
    const newPool: Pool = {
      ...pool,
      specimenIds: [...(pool.specimenIds || []), specimen.id],
    };

    setCount(newCount);
    setList(newList);
    setCurrent(newCurrent);
    setPool(newPool);
    setContainerId(null);
  };

  /**
   * Remove a list item
   *
   * @param {string} key - the key to be removed
   */
  function removeListItem(key: string) {

    const filter = (id) => id !== list[key].specimen.id;
    const newSpecimenIds = pool.specimenIds ? pool.specimenIds.filter(id => id
                                                                      !==
                                                                        list[key].specimen.id)
                                                                      : [];
    poolHandler.set('specimenIds', newSpecimenIds);

    // delete list at key.
    setList(prevList => {
      const newList = clone(prevList); // Assuming clone deeply copies the object
      delete newList[key];
      if (isEmpty(newList)) {
        setCurrent({});
      }
      return newList;
    });

    setContainerId(null); // Assuming containerId is managed with its own state
  }

  /**
   * Validate a list item for a container
   *
   * @param {string} containerId - the container being validated
   *
   * @return {Promise} - a resolved or rejected promise
   */
  const validateListItem = (containerId: string) => {
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];

    // Throw error if new list item does not meet requirements.
    if (!isEmpty(list) &&
      (
        specimen.candidateId !== current.candidateId ||
        specimen.sessionId !== current.sessionId ||
        specimen.typeId !== current.typeId ||
        container.centerId !== current.centerId
      )
    ) {
      Swal.fire({
        title: 'Oops!',
        text: 'Specimens must be of the same PSCID, ' +
              'Visit Label, Type and Center',
        type: 'warning',
      });
      return Promise.reject(new Error('Validation failed'));
    }

    return Promise.resolve();
  }

  // generate barcode list from list object.
  const barcodeList = Object.entries(list)
  .map(([key, item]) => {
    const removeItem = () => removeListItem(key);
    // I cannot get this to work in the css file.
    const style = {
      color: '#DDDDDD',
      marginLeft: 10,
      cursor: 'pointer',
    };
    return (
      <div key={key} className='preparation-item'>
        <div>{item.container.barcode}</div>
        <div
          className='glyphicon glyphicon-remove'
          onClick={removeItem}
          style={style}
        />
      </div>
    );
  });

  // Generate Pool form.
  const specimenUnits = mapFormOptions(options.specimen.units, 'label');

  const BarcodeInput = () => {
    // Restrict list of barcodes to only those that would be valid.
    const barcodesPrimary = Object.values(containers)
    .reduce((result: Record<string, string>, container: Container) => {
      if (options.container.types[container.typeId].primary == 1) {
        const specimen = specimens[container.specimenId];
        const inList = Object.values(list)
        .find((i) => i.container.id == container.id);
  
        if (specimen.quantity > 0 &&
            specimen.poolId == null &&
            !inList) {
          result[container.id] = container.barcode;
        }
      }
      return result;
    }, {});
  
    const handleInput = (name: string, containerId: string) => {
      if (containerId) {
        validateListItem(containerId)
        .then(() => setPoolList(containerId))
        .catch(error => Swal.fire('Oops!', error.message, 'warning')); //not sure if this is necessary
      }
    };

    return (
      <SearchableDropdown
        name={'containerId'}
        label={'Specimen'}
        onUserInput={handleInput}
        options={barcodesPrimary}
        value={containerId}
        errorMessage={errors.total}
      />
    );
  }

  const form = (
    <FormElement name="poolSpecimenForm">
      <div className='row'>
        <div className='col-sm-10 col-sm-offset-1'>
          <StaticElement
            label='Pooling Note'
            text="Select or Scan the specimens to be pooled. Specimens must
                  have a Status of 'Available', have a Quantity of greater
                  than 0, and share the same Type, PSCID, Visit Label
                  and Current Site. Pooled specimens cannot already belong to
                  a pool. Once pooled, the Status of specimen will be changed
                  to 'Dispensed' and there Quantity set to '0'"
          />
          <StaticElement
            label='Specimen Type'
            text={
              (options.specimen.types[current.typeId]||{}).label || '—'
            }
          />
          <StaticElement
            label='PSCID'
            text={
              (options.candidates[current.candidateId]||{}).pscid || '—'
            }
          />
          <StaticElement
            label='Visit Label'
            text={(options.sessions[current.sessionId]||{}).label || '—'}
          />
          <div className='row'>
            <div className='col-xs-6'>
              <h4>Barcode Input</h4>
              <div className='form-top'/>
              <BarcodeInput
                list={list}
                options={options}
                errors={errors}
                containerId={containerId}
                validateListItem={validateListItem}
                setPoolList={setPoolList}
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
          <div className='form-top'/>
          <TextboxElement
            name='label'
            label='Label'
            onUserInput={poolHandler.set}
            required={true}
            value={pool.label}
            errorMessage={errors.label}
          />
          <TextboxElement
            name='quantity'
            label='Quantity'
            onUserInput={poolHandler.set}
            required={true}
            value={pool.quantity}
            errorMessage={errors.quantity}
          />
          <SelectElement
            name='unitId'
            label='Unit'
            options={specimenUnits}
            onUserInput={poolHandler.set}
            required={true}
            value={pool.unitId}
            errorMessage={errors.unitId}
          />
          <DateElement
            name='date'
            label='Date'
            onUserInput={poolHandler.set}
            required={true}
            value={pool.date}
            errorMessage={errors.date}
          />
          <TimeElement
            name='time'
            label='Time'
            onUserInput={poolHandler.set}
            required={true}
            value={pool.time}
            errorMessage={errors.time}
          />
        </div>
      </div>
    </FormElement>
  );

  const onSubmit = (pool, list) => {                                         
 //   const stati = options.container.stati;                                   
 //   const dispensedId = Object.keys(stati)                                   
 //   .find(                                                                   
 //      (key) => stati[key].label === 'Dispensed'                             
 //      );                                                                    
 //   const update = Object.values(list)                                       
 //   .reduce((result, item) => {                                              
 //     item.container.statusId = dispensedId;                                 
 //     item.specimen.quantity = '0';                                          
 //     // XXX: By updating the container and specimen after, it's causing issues
 //     // if they don't meet validation. The error is being thrown only after the
 //     // pool has already been saved to the atabase! Not sure how to resolve this.
 //     // return [...result,                                                  
 //     //         () => this.updateContainer(item.container, false),          
 //     //         () => this.updateSpecimen(item.specimen, false),            
 //     //       ];                                                            
 //   }, []);                                                                  
                                                                               
 //   // const errors = this.validatePool(pool);                               
 //   // if (!isEmpty(errors)) {                                               
 //   //   return Promise.reject(errors);                                      
 //   // }                                                                     
                                                                               
 //   const pools = await post(pool, poolAPI, 'POST');                         
 //   setPools(pools);                                                         
  }   

  const handleSubmit = async () => {
    try {
      // Await the completion of the onSubmit operation.
      await onSubmit(pool, list);
      // Operation successful, implicitly resolves the promise returned by handleSubmit.
    } catch (errors) {
      // An error occurred, handle it, e.g., by setting error state.
      setErrors(errors);
      // Rethrow the error if you want to further handle it where handleSubmit is called.
      throw errors;
    }
  };

  return (
    <Modal
      title='Pool Specimens'
      show={show}
      onClose={onClose}
      onSubmit={handleSubmit}
      throwWarning={true}
    >
      {form}
    </Modal>
  );
}


export default PoolSpecimenForm;
