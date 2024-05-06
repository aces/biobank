import React, { ReactElement, useState, useEffect, useMemo } from 'react';
import { ProcessForm, SpecimenField, ContainerField} from '../components';
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
  ContainerProvider,
} from '../entities';
import Form from 'Form';
const {
  SelectElement,
  StaticElement,
  FormElement,
  SearchableDropdown,
  CheckboxElement,
} = Form

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
   * Remove a list item from a container
   *
   * FIXME: SHOULD LIKELY GO INTO A HIGHER LEVEL COMPONENT
   *
   * @param {string} key - the key to remove
   */
  const removeListItem = (key) => {
    // const newList = clone(list);
    // delete newList[key];
    // const newTypeId = isEmpty(newList) ? null : clone(typeId);
    // setList(list);
    // setTypeId(newTypeId);
  }

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

  // const globalForm = typeId ? (
  //   <EditForm>
  //     <SpecimenField property={'quantity'} specimen={specimen}/>
  //     <SpecimenField property={'unitId'} specimen={specimen}/>
  //     {options.specimen.types[typeId].freezeThaw == 1 ? (
  //       <SpecimenField property={"fTCycle"} specimen={specimen}/>
  //     ) : null}
  //     <SpecimenField property={'unitId'} specimen={specimen}/>
  //     <ContainerField property={'typeId'} container={container}/>
  //     <ContainerField property={'lotNumber'} container={container}/>
  //     <ContainerField property={'statusId'} container={container}/>
  //     <SpecimenField property={'projectIds'} specimen={specimen}/>
  //   </EditForm>
  // ) : null;

  const collectionForm = editable.collection ? (
    <div>
      <StaticElement
        label='Protocol'
        text={collection.protocol}
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
      <StaticElement
        label='Protocol'
        text={preparation.protocol}
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

  // const tabList = [{
  //   id: 'global',
  //   label: 'Global',
  //   error: !isEmpty(specimen.errors) || !isEmpty(container.errors),
  //   content: globalForm,
  // }];
  // // TODO: replace with show state eventually...
  // if (true) {
  //   tabList.push({
  //     id: 'collection',
  //     label: 'Collection',
  //     error: !isEmpty(specimen.collection.errors),
  //     content: collectionForm,
  //   });
  // }
  // // TODO: replace with show state eventually...
  // if (true) {
  //   tabList.push({
  //     id: 'preparation',
  //     label: 'Preparation',
  //     content: preparationForm,
  //   });
  // }
  // const tabContent = tabList
  // .map((tab, i) => <TabPane key={i} TabId={tab.id}>{tab.content}</TabPane>);

  const handlePoolInput = (name, value) => value && pool.set(name, value);
  const handleClose = () => onClose;

  // TODO: This should likely be cleaned up because there must be a more
  // efficient way of structuring it.
  const handleSubmit = () => {
    // this.setState({errors: {container: {}, specimen: {}}});
    // const prepList = Array.from(specimens.values()).map((item) => {
    //   const specimen = clone(item.specimen);
    //   const container = clone(item.container);

    //   // Clone values from global specimen.
    //   Object.keys(specimen).forEach((name) => {
    //     if (specimen[name] != null) {
    //       specimen[name] = specimen[name];
    //     }
    //   });

    //   // Clone values from global container.
    //   Object.keys(container).forEach((name) => {
    //     if (container[name] != null) {
    //       container[name] = container[name];
    //     }
    //   });

    //   // Clone collection values to specimen.
    //   Object.keys(collection).forEach((name) => {
    //     if (typeof specimen.collection[name] === 'object' &&
    //         specimen.collection[name] !== null) {
    //       Object.keys(collection[name]).forEach((index) => {
    //         if (collection[name][index] != null) {
    //           specimen.collection[name][index] = collection[name][index];
    //         }
    //       });
    //     } else {
    //       if (collection[name] != null) {
    //         specimen.collection[name] = collection[name];
    //       }
    //     }
    //   });

    //   // Clone specimen values to specimen.
    //   Object.keys(preparation).forEach((name) => {
    //     if (typeof specimen.preparation[name] === 'object' &&
    //         specimen.preparation[name] !== null) {
    //       Object.keys(preparation[name]).forEach((index) => {
    //         if (preparation[name][index] != null) {
    //           specimen.preparation[name][index] = preparation[name][index];
    //         }
    //       });
    //     } else {
    //       if (preparation[name] != null) {
    //         specimen.preparation[name] = preparation[name];
    //       }
    //     }
    //   });

    //   return {specimen, container};
    // });

    // return new Promise((resolve, reject) => {
      // onSubmit(prepList)
      // .then(() => resolve(), (errors) => this.setState({errors}, reject()));
    // });
  };

  // const editForms = specimens.size > 1 ? (
  //   <div className='form-top'>
  //     <StaticElement
  //       label='Editing Note'
  //       text={dict.noteForEdit}
  //     />
  //     <VerticalTabs
  //       tabs={tabList}
  //       onTabChange={(id) => edit(id)}
  //       updateURL={false}
  //     >
  //       {tabContent}
  //     </VerticalTabs>
  //   </div>
  // ) : null;

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
      <Form.InputList label={'Barcode'} list={specimens} options={context.specimens}/>
      {/*{editForms}*/}
    </Modal>
  );
}

interface FieldProps {
  entity: Entity<any>;
  property: keyof any;
}

const EditForm: React.FC = ({
  children,
}) => {
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
              <CheckboxElement
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
