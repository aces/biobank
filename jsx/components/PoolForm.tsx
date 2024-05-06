import Modal from 'Modal';
import React, { useState, ReactElement} from 'react';
import { mapFormOptions, clone, isEmpty } from '../utils';
import { useBiobankContext } from '../hooks';
import { PoolField, SpecimenField } from '../components';
import { IPool, Pool, usePool, ISpecimen, useSpecimen, useEntities } from '../entities';
import dict from '../i18n/en.json';
import Swal from 'sweetalert2';
import Form from 'Form';
const {
  FormElement,
  SearchableDropdown,
  StaticElement,
} = Form;

/**
 * React component with form for entering pool specimens
 */
const PoolForm: React.FC<{
  onClose: () => void,
  show: boolean,
}> = ({
  onClose,
  show,
}) => {
  const pool = usePool();
  const pools = useEntities(Pool);
  const specimen = useSpecimen();
  const [count, setCount] = useState<number>(0);
  const [errors, setError] = useState<Record<string, any>>({});

  const { options, containers, specimens } = useBiobankContext();

  /**
   * Sets the current pool list being displayed?
   *
   * @param {int} container - container with pool?
   */
  const setPoolList = (barcode: string) => {
    //increase count
    const newCount = count + 1;

    // set specimen and container
    const newSpecimen = specimens[barcode];
    
    // set current global values
    if (pools.isEmpty) {
      specimen.replace(newSpecimen);
    }

    // set list values
    pools.add(newSpecimen);

    setCount(newCount);
    pool.set('specimens', [...(pool.specimens || []), newSpecimen.barcode]);
  };

  /**
   * Remove a list item
   *
   * @param {string} key - the key to be removed
   */
  function removeListItem(key: string) {

    const filter = (id) => id !== pools.entities[key].specimen.id;
    const newSpecimens = pool.specimens ? pool.specimens.filter(id => id
                                                                      !==
                                                                        pools.entities[key].specimen.id)
                                                                      : [];
    pool.set('specimens', newSpecimens);

    pools.remove(key);

    if (pools.isEmpty()) {
        specimen.clear();
    }
  }

  /**
   * Validate a list item for a container
   *
   * @param {string} containerId - the container being validated
   *
   * @return {Promise} - a resolved or rejected promise
   */
  //const validateListItem = (barcode: string) => {
    // const container = containers[containerId];
    // const specimen = specimens[container.specimenId];

    // // Throw error if new list item does not meet requirements.
    // if (pools.size === 0) &&
    //   (
    //     specimen.candidateId !== specimen.candidateId ||
    //     specimen.sessionId !== specimen.sessionId ||
    //     specimen.typeId !== specimen.typeId ||
    //     container.centerId !== specimen.container.centerId
    //   )
    // ) {
    //   Swal.fire({
    //     title: 'Oops!',
    //     text: 'Specimens must be of the same PSCID, ' +
    //           'Visit Label, Type and Center',
    //     type: 'warning',
    //   });
    //   return Promise.reject(new Error('Validation failed'));
    // }

    // return Promise.resolve();
  //}

  // generate barcode list from list object.
  const barcodeList = Array.from(pools.entities)
  .map(([key, pool]) => {
    const removeItem = () => removeListItem(key);
    // I cannot get this to work in the css file.
    const style = {
      color: '#DDDDDD',
      marginLeft: 10,
      cursor: 'pointer',
    };
    const { label } = pool.getData();
    return (
      <div key={key} className='preparation-item'>
        <div>{label}</div>
        <div
          className='glyphicon glyphicon-remove'
          onClick={removeItem}
          style={style}
        />
      </div>
    );
  });

  // Generate Pool form.
  const BarcodeInput = () => {
    // Restrict list of barcodes to only those that would be valid.
    // Step 1: Compile a list/set of all barcodes already included in pools
    const pooledSpecimens = new Set();
    pools.entities.forEach(pool => {
      pool.getData().specimens.forEach(barcode => pooledSpecimens.add(barcode));
    });
    
    // Step 2: Create an options object for the searchable dropdown
    const dropdownOptions = Object.values(specimens).reduce((acc, specimen:
                                                             ISpecimen) => {
      // Check if the specimen is not in a pool, its quantity is more than 0, and it doesn't have a poolId
      if (!pooledSpecimens.has(specimen.barcode) && specimen.quantity > 0 && specimen.pool == null) {
        // The dropdown options object where key is the specimen ID and value is the barcode
        acc[specimen.id] = `${specimen.barcode} (${specimen.quantity} available)`;
      }
      return acc;
    }, {});
  
    const handleInput = (name: string, barcode: string) => {
      if (barcode) {
        // TODO: this needs to be rewritten..
        //validateListItem(barcode);
        //.then(() => pool.set('specimens', [...pool.specimens, barcode]))
      }
    };
    const barcodesPrimary = {barcode: 'barcode'};

    return (
      <SearchableDropdown
        name={'barcode'}
        label={'Specimen'}
        onUserInput={handleInput}
        options={barcodesPrimary}
        value={specimen.barcode}
        errorMessage={errors.total}
      />
    );
  }

  const onSubmit = (pool, pools) => {                                         
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
      await onSubmit(pool, pools);
      // Operation successful, implicitly resolves the promise returned by handleSubmit.
    } catch (errors) {
      // An error occurred, handle it, e.g., by setting error state.
      // setErrors(errors);
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
      <FormElement name="poolSpecimenForm">
        <div className='row'>
          <div className='col-sm-10 col-sm-offset-1'>
            <StaticElement label='Pooling Note' text={dict.noteForPools}/>
            <SpecimenField property={'type'} isStatic/>
            <SpecimenField property={'candidate'} isStatic/>
            <SpecimenField property={'session'} isStatic/>
            <div className='row'>
              <div className='col-xs-6'>
                <h4>Barcode Input</h4>
                <div className='form-top'/>
                <BarcodeInput/>
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
            <PoolField property={'label'} pool={pool}/>
            <PoolField property={'quantity'} pool={pool}/>
            <PoolField property={'unit'} pool={pool}/>
            <PoolField property={'date'} pool={pool}/>
            <PoolField property={'time'} pool={pool}/>
          </div>
        </div>
      </FormElement>
    </Modal>
  );
}


export default PoolForm;
