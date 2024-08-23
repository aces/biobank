import React, { ReactNode, ReactElement, useState, useEffect, useMemo } from 'react';
import { ProcessForm, SpecimenField, ContainerField, InputList } from '../forms';
import { VerticalTabs, TabPane } from 'Tabs';
import Modal from 'Modal';
import Loader from 'Loader';
import { mapFormOptions, clone, isEmpty } from '../utils';
import { useBiobankContext, useEditable } from '../hooks';
import dict from '../i18n/en.json';
import {
  Specimen,
  ISpecimen,
  IContainer,
  EntitiesHook,
  Entity,
  useContainer,
  useSpecimen,
  usePool,
  useProcess,
  useEntities,
  SpecimenProvider,
  ContainerProvider,
} from '../entities';
import { StaticField, CheckboxField } from './Form';
import Swal from 'sweetalert2';

const BatchEditForm: React.FC<{
  show: boolean,
  onClose: () => void,
}> = ({
  show,
  onClose,
}) => {
  const container = useContainer();
  const specimen = useSpecimen();

  const pool = usePool();
  const specimens = useEntities(Specimen);
  const [type, setType] = useState();
  const collection = useProcess();
  const preparation = useProcess();
  const [count, setCount] = useState(0);

  const { options, pools, ...context } = useBiobankContext();
  const { editable, edit, clear } = useEditable({global: true});

  /**
   * Add a new list item to a container
   *
   * FIXME: SHOULD LIKELY GO INTO A HIGHER LEVEL COMPONENT
   *
   * @param {int} container - the container to add an item to
   */
  // const addListItem = (containerBarcode) => {
    // const newList = clone(list);
    // const newCollection = clone(collection);
    // const newPreparation = clone(preparation);
    // const newCount = clone(count) + 1;
    // const newShow = clone(show);

    // // Set Specimen and Container.
    // const container = containers[containerId];
    // const specimen = specimens[container.specimenId];

    // // Set current global values.
    // const newTypeId = specimen.typeId;

    // // Set list values.
    // newList[newCount] = {specimen, container};

    // // This determines if every specimen in the list has the same collection
    // // protocol.
    // newShow.collection = Object.keys(newList).length > 1 && Object.values(newList)
    // .every((item, i, listArray) => {
    //   return item.specimen.collection &&
    //     item.specimen.collection.protocolId ===
    //     listArray[0].specimen.collection.protocolId;
    // });

    // // If so, set the collection protocolId.
    // if (newShow.collection) {
    //   newCollection.protocolId = newList[
    //     Object.keys(newList)[0]
    //   ].specimen.collection.protocolId;
    // }

    // // This determines if every specimen in the list has the same preparation
    // // protocol.
    // newShow.preparation = Object.keys(newList).length > 1 && Object.values(newList)
    // .every((item, i, listArray) => {
    //   return item.specimen.preparation &&
    //     item.specimen.preparation.protocolId ===
    //     listArray[0].specimen.preparation.protocolId;
    // });

    // // If so, set the preparation protocolId.
    // if (newShow.preparation) {
    //   newPreparation.protocolId = newList[
    //     Object.keys(newList)[0]
    //   ].specimen.preparation.protocolId;
    // }

    // setList(newList);
    // setCollection(newCollection);
    // setPreparation(newPreparation);
    // setCount(newCount);
    // setShow(newShow);
  // }

  /**
   * Set a pool?
   *
   * @param {string} name - the pool name
   * @param {int} poolId - the pool id
   */
  // const setPool = (name, pool) => {
    // const pool = clone(pools[pool]);

    // // This struture is what allows pools to be loaded and then have the pool
    // // label disappear once the barcodes have been added to the list.
    // setLoading(true);
    // setPool(pool)
    // .then(() => Promise.all(pool.specimens
    //   .map((specimen) => Object.values(list)
    //     .find((item) => item.specimen.id === specimen)
    //     || addListItem(specimens[specimen].container))
    //   .map((p) => p instanceof Promise ? p : Promise.resolve(p))))
    // .then(() => setPoolId(null))
    // .then(() => setLoading(false));
  //}

  const globalForm = type ? (
    <EditForm>
      <SpecimenProvider specimen={specimen}>
        <SpecimenField property={'quantity'}/>
        <SpecimenField property={'unit'}/>
        {type.freezeThaw == 1 ? (
          <SpecimenField property={"fTCycle"}/>
        ) : null}
        <ContainerField property={'type'}/>
        <ContainerField property={'lotNumber'}/>
        <ContainerField property={'status'}/>
        <SpecimenField property={'projects'}/>
      </SpecimenProvider>
    </EditForm>
  ) : null;

  const collectionForm = editable.collection ? (
    <div>
      <StaticField
        label='Protocol'
        value={collection.protocol.label}
      />
      <EditForm>
        <ProcessForm
          edit={true}
          process={collection}
          processStage='collection'
          type={type}
          hideProtocol={true}
          />
      </EditForm>
    </div>
  ) : null;

  const preparationForm = editable.preparation ? (
    <div>
      <StaticField
        label='Protocol'
        value={preparation.protocol.label}
      />
      <EditForm>
        <ProcessForm
          edit={true}
          process={preparation}
          processStage='preparation'
          type={type}
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

  const BarcodeList = ({ specimens }) => {
    const barcodeList = useMemo(() => {
      let items = [];

      specimens.forEach((specimen, key) => {
        const { barcode } = specimen.getData().container;
        items.push(
          <div key={key} className='preparation-item'>
            <div>{barcode}</div>
            <div
              className='glyphicon glyphicon-remove'
              style={glyphStyle}
              onClick={() => specimens.remove(key)}
            />
          </div>
        );
      });

      return items;
    }, [specimens]);

    return <div>{barcodeList}</div>;
  };    

  const tabList = [{
    id: 'global',
    label: 'Global',
    error: !isEmpty(specimen.errors) || !isEmpty(container.errors),
    content: globalForm,
  }];
  // TODO: replace with show state eventually...
  if (true) {
    tabList.push({
      id: 'collection',
      label: 'Collection',
      error: !isEmpty(specimen.collection.errors),
      content: collectionForm,
    });
  }
  // TODO: replace with show state eventually...
  if (true) {
    tabList.push({
      id: 'preparation',
      label: 'Preparation',
      error: !isEmpty(specimen.preparation.errors),
      content: preparationForm,
    });
  }
  const tabContent = tabList
  .map((tab, i) => <TabPane key={i} TabId={tab.id}>{tab.content}</TabPane>);

  const handlePoolInput = (name, value) => value && pool.set(name, value);
  const handleClose = () => onClose;
  const handleSubmit = async (): Promise<void> => {
    // Create a list of prepared specimens
    const prepList = Array.from(specimens.entities.values()).map((specimen) => {
      // Clone specimen values
      const clonedSpecimen = { ...specimen };
  
      // Clone values from global container
      const clonedContainer = { ...container };
  
      // Clone collection values to specimen
      clonedSpecimen.collection = { ...specimen.collection, ...collection };
  
      // Clone preparation values to specimen
      clonedSpecimen.preparation = { ...specimen.preparation, ...preparation };
  
      return { specimen: clonedSpecimen, container: clonedContainer };
    });
  
    try {
      // Call the onSubmit function with the prepared list
      await onSubmit(prepList);
    } catch (errors) {
      // Handle errors (you can set errors in state or handle them accordingly)
      console.error('Error submitting data:', errors);
    }
  };


  const editForms = specimens.size > 1 ? (
    <div className='form-top'>
      <StaticField label='Editing Note' value={dict.noteForEdit}/>
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
      show={show}
      onClose={handleClose}
      onSubmit={specimens.entities.size > 1 && handleSubmit}
      throwWarning={true}
    >
      <StaticElement
        label='Editing Note'
        text="Select or Scan the specimens to be edited. Specimens
              must share the same Type."
      /> 
      <SpecimenField property={'type'} isStatic/>
      <InputList label={'Barcode'} list={specimens} options={context.specimens}/>
      {/*{editForms}*/}
    </Modal>
  );
}

interface FieldProps {
  entity: Entity<any>;
  property: keyof any;
}

const EditForm = ({
  children,
}: {children: ReactNode}) => {
  const editForm = React.Children.map(children, (child: ReactElement<FieldProps>) => {
    // if (!React.isValidElement(child)) return null;

    const handleClick = (name, value) => {
      if (!value) {
        child.props.entity.set(name, null);
      }
      if (value && child.props.entity[child.props.property] == null) {
        child.props.entity.set(name, '');
      }
    };

    // XXX: need to find a way to make these fields not required because I
    // remove this line: {React.cloneElement(child, {required: false})}
    return React.isValidElement(child) && typeof child.type === 'function' && (
      <div className="row">
        <div className="col-xs-12">
          <div className="row">
            <div className="col-xs-10">
              {React.cloneElement(child)}
            </div>
            <div className="col-xs-2">
              <CheckboxField
                name={child.props.property}
                value={child.props.entity[child.props.property] != null}
                onUserInput={handleClick}
              />
            </div>
          </div>
        </div>
      </div>
    );
  });

  return <>{editForm}</>;
}

export default BatchEditForm;
