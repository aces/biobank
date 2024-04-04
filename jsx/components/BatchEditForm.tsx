import { useState } from 'react';
import { ProcessForm } from '../components';
import { VerticalTabs, TabPane } from 'Tabs';
import Modal from 'Modal';
import Loader from 'Loader';
import { mapFormOptions, clone, isEmpty } from '../utils';
import { useBiobankContext, useEditable, useContainer, useSpecimen } from '../hooks';

import Swal from 'sweetalert2';

const initialState = {
  collection: {},
  preparation: {},
  list: {},
  count: 0,
  current: {},
  errors: {specimen: {}, container: {}},
  loading: false,
  editable: {global: true},
  show: {collection: false, preparation: false},
};

/**
 * Biobank batch edit specimen form
 */
const BatchEditForm: React.FC<{
  show: boolean,
  onClose: () => void,
}> = ({
  show: showPROP,
  onClose,
}) => {
  const container = useContainer();
  const specimen = useSpecimen();

  const [poolId, setPoolId] = useState();
  const [list, setList] = useState({});
  const [typeId, setTypeId] = useState();
  const [errors, setErros] = useState();
  const [collection, setCollection] = useState({});
  const [preparation, setPreparation] = useState({});
  const [count, setCount] = useState({});
  const [show, setShow] = useState({});
  const [loading, setLoading] = useState(false);

  const { pools, containers, specimens } = useBiobankContext();
  const { editable, edit, clear } = useEditable();

  /**
   * Add a new list item to a container
   *
   * FIXME: SHOULD LIKELY GO INTO A HIGHER LEVEL COMPONENT
   *
   * @param {int} containerId - the container to add an item to
   */
  const addListItem = (containerId) => {
    const newList = clone(list);
    const newCollection = clone(collection);
    const newPreparation = clone(preparation);
    const newCount = clone(count);
    const newShow = clone(show);
    const newTypeId = clone(typeId);

    // Increase count.
    newCount++;

    // Set Specimen and Container.
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];

    // Set current global values.
    newTypeId = specimen.typeId;

    // Set list values.
    newList[newCount] = {specimen, container};

    // This determines if every specimen in the list has the same collection
    // protocol.
    newShow.collection = Object.keys(newList).length > 1 && Object.values(newList)
    .every((item, i, listArray) => {
      return item.specimen.collection &&
        item.specimen.collection.protocolId ===
        listArray[0].specimen.collection.protocolId;
    });

    // If so, set the collection protocolId.
    if (newShow.collection) {
      newCollection.protocolId = newList[
        Object.keys(newList)[0]
      ].specimen.collection.protocolId;
    }

    // This determines if every specimen in the list has the same preparation
    // protocol.
    newShow.preparation = Object.keys(newList).length > 1 && Object.values(newList)
    .every((item, i, listArray) => {
      return item.specimen.preparation &&
        item.specimen.preparation.protocolId ===
        listArray[0].specimen.preparation.protocolId;
    });

    // If so, set the preparation protocolId.
    if (newShow.preparation) {
      newPreparation.protocolId = newList[
        Object.keys(newList)[0]
      ].specimen.preparation.protocolId;
    }

    setList(newList);
    setCollection(newCollection);
    setPreparation(newPreparation);
    setCount(newCount);
    setShow(newShow);
  }

  /**
   * Remove a list item from a container
   *
   * FIXME: SHOULD LIKELY GO INTO A HIGHER LEVEL COMPONENT
   *
   * @param {string} key - the key to remove
   */
  const removeListItem = (key) => {
    const newList = clone(list);
    delete newList[key];
    const newTypeId = isEmpty(newList) ? null : clone(typeId);
    setList(list);
    setTypeId(newTypeId);
  }

  /**
   * Set a pool?
   *
   * @param {string} name - the pool name
   * @param {int} poolId - the pool id
   */
  const setPool = (name, poolId) => {
    const pool = clone(pools[poolId]);

    // This struture is what allows pools to be loaded and then have the pool
    // label disappear once the barcodes have been added to the list.
    setLoading(true);
    setPoolId(poolId)
    .then(() => Promise.all(pool.specimenIds
      .map((specimenId) => Object.values(list)
        .find((item) => item.specimen.id === specimenId)
        || addListItem(specimens[specimenId].containerId))
      .map((p) => p instanceof Promise ? p : Promise.resolve(p))))
    .then(() => setPoolId(null))
    .then(() => setLoading(false));
  }

  /**
   * Validate the list items for a container
   *
   * @param {int} containerId - the container to validate
   *
   * @return {Promise}
   */
  const validateListItem = (containerId) => {
    const container = containers[containerId];
    const specimen = specimens[container.specimenId];
    if (!isEmpty(list) &&
      (specimen.typeId !== typeId)) {
      Swal.fire(
        'Oops!',
        'Specimens must be of the same Type and Center',
        'warning'
      );
      return Promise.reject();
    }
    return Promise.resolve();
  }

  if (loading) {
    return <Loader/>;
  }

  const containerTypesPrimary = mapFormOptions(
    options.container.typesPrimary,
    'label',
  );
  const containerTypes = {};
  if (typeId && options.specimen.typeContainerTypes[typeId]) {
    Object.keys(containerTypesPrimary).forEach((id) => {
      options.specimen.typeContainerTypes[typeId].forEach((i) => {
        if (id == i) {
          containerTypes[id] = containerTypesPrimary[id];
        }
      });
    });
  }
  const globalForm = typeId ? (
    <EditForm>
      <SpecimenField property={'quantity'} specimen={specimen}/>
      <SpecimenField property={'unitId'} specimen={specimen}/>
      {options.specimen.types[typeId].freezeThaw == 1 ? (
        <SpecimenField property={"fTCycle"} specimen={specimen}/>
      ) : null}
      <SpecimenField property={'unitId'} specimen={specimen}/>
      <SelectElement
        name='typeId'
        label='Container Type'
        value={container.typeId}
        options={containerTypes}
        errorMessage={errors.container.typeId}
        onUserInput={container.set}
      />
      <ContainerField property={'lotNumber'} container={container}/>
      <ContainerField property={'statusId'} container={container}/>
      <SpecimenField property={'projectIds'} specimen={specimen}/>
    </EditForm>
  ) : null;

  const collectionForm = editable.collection ? (
    <div>
      <StaticElement
        label='Protocol'
        text={options.specimen.protocols[collection.protocolId].label}
      />
      <EditForm>
        <ProcessForm
          edit={true}
          errors={errors.specimen.collection || {}}
          process={collection}
          processStage='collection'
          setParent={specimen.setProcess} // TODO: might need to specific the process
          typeId={typeId}
          hideProtocol={true}
          />
      </EditForm>
    </div>
  ) : null;

  const preparationForm = editable.preparation ? (
    <div>
      <StaticElement
        label='Protocol'
        text={options.specimen.protocols[preparation.protocolId].label}
      />
      <EditForm>
        <ProcessForm
          edit={true}
          errors={errors.specimen.preparation || {}}
          process={preparation}
          processStage='preparation'
          setParent={specimen.setProcess}
          typeId={typeId}
          hideProtocol={true}
          />
      </EditForm>
    </div>
  ) : null;

  // TODO: This should likely be filtered so that only pools that match the
  // proper criteria are left in the list.
  const mappedPools = mapFormOptions(pools, 'label');
  const glyphStyle = {
    color: '#DDDDDD',
    marginLeft: 10,
    cursor: 'pointer',
  };

  const barcodeList = Object.entries(list)
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

  const tabList = [{
    id: 'global',
    label: 'Global',
    error: !isEmpty(errors.specimen) || !isEmpty(errors.container),
    content: globalForm,
  }];
  if (show.collection) {
    tabList.push({
      id: 'collection',
      label: 'Collection',
      error: !isEmpty(errors.specimen.collection),
      content: collectionForm,
    });
  }
  if (show.preparation) {
    tabList.push({
      id: 'preparation',
      label: 'Preparation',
      content: preparationForm,
    });
  }
  const tabContent = tabList
  .map((tab, i) => <TabPane key={i} TabId={tab.id}>{tab.content}</TabPane>);

  const handlePoolInput = (name, value) => value && setPool(name, value);
  const handleClose = () => onClose;

  // TODO: This should likely be cleaned up because there must be a more
  // efficient way of structuring it.
  const handleSubmit = () => {
    // this.setState({errors: {container: {}, specimen: {}}});
    const prepList = Object.values(list).map((item) => {
      const specimen = clone(item.specimen);
      const container = clone(item.container);

      // Clone values from global specimen.
      Object.keys(specimen).forEach((name) => {
        if (specimen[name] != null) {
          specimen[name] = specimen[name];
        }
      });

      // Clone values from global container.
      Object.keys(container).forEach((name) => {
        if (container[name] != null) {
          container[name] = container[name];
        }
      });

      // Clone collection values to specimen.
      Object.keys(collection).forEach((name) => {
        if (typeof specimen.collection[name] === 'object' &&
            specimen.collection[name] !== null) {
          Object.keys(collection[name]).forEach((index) => {
            if (collection[name][index] != null) {
              specimen.collection[name][index] = collection[name][index];
            }
          });
        } else {
          if (collection[name] != null) {
            specimen.collection[name] = collection[name];
          }
        }
      });

      // Clone specimen values to specimen.
      Object.keys(preparation).forEach((name) => {
        if (typeof specimen.preparation[name] === 'object' &&
            specimen.preparation[name] !== null) {
          Object.keys(preparation[name]).forEach((index) => {
            if (preparation[name][index] != null) {
              specimen.preparation[name][index] = preparation[name][index];
            }
          });
        } else {
          if (preparation[name] != null) {
            specimen.preparation[name] = preparation[name];
          }
        }
      });

      return {specimen, container};
    });

    return new Promise((resolve, reject) => {
      // onSubmit(prepList)
      // .then(() => resolve(), (errors) => this.setState({errors}, reject()));
    });
  };

  const editForms = Object.keys(list).length > 1 ? (
    <div className='form-top'>
      <StaticElement
        label='Editing Note'
        text="Select a form for the list to
              edit the specimen values. Any previous value associated
              with a Specimen for a given field will be
              overwritten if one is added on this form."
      />
      <VerticalTabs
        tabs={tabList}
        onTabChange={(id) => edit(id)}
        updateURL={false}
      >
        {tabContent}
      </VerticalTabs>
    </div>
  ) : null;

  return (
    <Modal
      title='Edit Specimens'
      show={showPROP}
      onClose={handleClose}
      onSubmit={Object.keys(list).length > 1 && handleSubmit}
      throwWarning={true}
    >
      <FormElement>
        <div className='row'>
          <div className='col-sm-10 col-sm-offset-1'>
            <StaticElement
              label='Editing Note'
              text="Select or Scan the specimens to be edited. Specimens
                    must share the same Type."
            />
            <StaticElement
              label='Specimen Type'
              text={(options.specimen.types[typeId]||{}).label || '—'}
            />
            <div className='row'>
              <div className='col-xs-6'>
                <h4>Barcode Input</h4>
                <div className='form-top'/>
                <BarcodeInput
                  data={data}
                  options={options}
                  list={list}
                  validateListItem={validateListItem}
                  addListItem={addListItem}
                />
                <SearchableDropdown
                  name={'poolId'}
                  label={'Pool'}
                  onUserInput={handlePoolInput}
                  options={mappedPools}
                  value={poolId}
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
    </Modal>
  );
  
}

BatchEditForm.propTypes = {
};

/**
 * Input form to enter a barcode
 */
const BarcodeInput: React.FC<{
  list,
  addListItem,
  validateListItem
}> = ({
  list,
  addListItem,
  validateListItem
}) => {

  const [barcode, setBarcode] = useState();
  const [barcodesPrimary, setBarcodesPrimary] = useState();
  const { containers, options } = useBiobankContext();

  /**
   * Constructor
   *
   * @param {object} props - React props
   */
  useEffect(() => {
    const barcodesPrimary = Object.values(containers)
    .reduce((result, container) => {
      if (options.container.types[container.typeId].primary == 1) {
        const inList = Object.values(list)
        .find((i) => i.container.id == container.id);

        if (!inList) {
          result[container.id] = container.barcode;
        }
      }
      return result;
    }, {});

    setBarcode(null);
    setBarcodesPrimary(barcodesPrimary);
  });

  const handleInput = (name, value) => {
    setBarcode(value);
    const containerId = Object.keys(barcodesPrimary)
    .find((id) => barcodesPrimary[id] == value);
    containerId && validateListItem(containerId)
    .then(() => addListItem(containerId))
    .then(() => setBarcode(null));
  };

  return (
    <TextboxElement
      name={'barcode'}
      label={'Specimen'}
      value={barcode}
      onUserInput={handleInput}
    />
  );
  // <SearchableDropdown
  //   name={'containerId'}
  //   label={'Search Specimen'}
  //   onUserInput={onSearch}
  //   options={barcodesPrimary}
  //   value={containerId}
  // />
}

/**
 * @param {object} props
 * @return {*}
 */
const EditForm: React.FC<{
}> = ({
  children,
}) => {
  return React.Children.map(children, (child) => {
    const handleClick = (name, value) => {
      if (!value) {
        child.props.onUserInput(name, null);
      }
      if (value && child.props.value == null) {
        child.props.onUserInput(name, '');
      }
    };
    return React.isValidElement(child) && typeof child.type === 'function' && (
      <div className="row">
        <div className="col-xs-12">
          <div className="row">
            <div className="col-xs-10">
              {React.cloneElement(child, {required: false})}
            </div>
            <div className="col-xs-2">
              <CheckboxElement
                name={child.props.name}
                value={child.props.value != null}
                onUserInput={handleClick}
              />
            </div>
          </div>
        </div>
      </div>
    );
  });
}

export default BatchEditForm;
