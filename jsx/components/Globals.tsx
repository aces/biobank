import React, { useState, ReactElement, ReactNode} from 'react';
import { Link } from 'react-router-dom';
import { mapFormOptions, isEmpty } from '../utils';
import Modal from 'Modal';
import Loader from 'Loader';
import { ContainerParentForm, SpecimenField, ContainerField, CoordinateLabel } from '../components';
import { Button } from './';
import { useSpecimenContext, useContainerContext } from '../entities';
import { useBarcodePageContext, useBiobankContext } from '../hooks';
import { ContainerAPI, SpecimenAPI } from '../APIs';

declare const loris: any;

const Globals: React.FC<{
  clearAll: () => void,
}> = ({ clearAll }) => {
  const { editable, edit } = useBarcodePageContext();
  const { options } = useBiobankContext();
  const specimen = useSpecimenContext();
  const container = useContainerContext();

  const editContainerType = loris.userHasPermission('biobank_specimen_alter')
    && specimen && (
    () => {
      edit('containerType');
    }
  );
  const containerTypeField = (
    <InlineField
      label={'Container Type'}
      updateValue={() => ContainerAPI.update(container)}
      clearAll={clearAll}
      pencil={true}
      value={container.type} //TODO remove '?'
      edit={editContainerType}
      editable={editable.containerType}
    >
      <ContainerField property={'type'}/> //TODO: this will have to be modified to type primary
    </InlineField>
  );

  const poolField = (specimen||{}).pool ? (
    <InlineField
      label='Pool'
      value={specimen.poolLabel}
    />
  ) : null;

  const quantityField = !isEmpty(specimen) ? (
    <InlineField
      label='Quantity'
      clearAll={clearAll}
      updateValue={() => SpecimenAPI.update(specimen)}
      edit={() => edit('quantity')}
      value={Math.round(specimen.quantity * 100) / 100+
      ' '+options.specimen.units[specimen.unit].label}
      editable={editable.quantity}
    >
      <SpecimenField property={'quantity'}/>
      <SpecimenField property={'unit'}/>
    </InlineField>
  ) : null;

  const fTCycleField = () => {
    if (!isEmpty(specimen)
        && specimen.fTCycle) {
      // const changeCycle = (value) => {
      //   editSpecimen(specimen)
      //   .then(() => {
      //     let cycle = specimen.fTCycle;
      //     cycle = cycle+value;
      //     setSpecimen('fTCycle', cycle);
      //   })
      //   .then(()=>updateSpecimen(specimen));
      // };
      // const increaseCycle = () => changeCycle(1);
      // const decreaseCycle = () => changeCycle(-1);
      // const updateFTCycle = loris.userHasPermission('biobank_specimen_update') ? (
      //   <div>
      //     {specimen.fTCycle > 0 ? (
      //       <div className='action' title='Remove Cycle'>
      //         <span
      //           className='action-button update'
      //           onClick={decreaseCycle}
      //         >
      //           <span className='glyphicon glyphicon-minus'/>
      //         </span>
      //       </div>
      //     ) : null}
      //     <div className='action' title='Add Cycle'>
      //       <span className='action-button update' onClick={increaseCycle}>
      //         <span className='glyphicon glyphicon-plus'/>
      //       </span>
      //     </div>
      //   </div>
      // ) : null;

      const editFTCycle = () => edit('fTCycle');
      return (
        <InlineField
          label={'Freeze-Thaw Cycle'}
          clearAll={clearAll}
          updateValue={() => SpecimenAPI.update(specimen)}
          edit={editFTCycle}
          value={specimen.fTCycle || 0}
          editable={editable.fTCycle}
        >
          <SpecimenField property={'fTCycle'}/>
        </InlineField>
      );
    }
  };

  const editTemperature = () => edit('temperature');
  const temperatureField = (
    <InlineField
      label={'Temperature'}
      clearAll={clearAll}
      updateValue={() => ContainerAPI.update(container)}
      edit={!container.parentContainer && editTemperature}
      value={container.temperature + '°'}
      editable={editable.temperature}
    >
      <ContainerField property={'temperature'}/>
    </InlineField>
  );

  const statusField = (
    <InlineField
      label={'Status'}
      clearAll={clearAll}
      updateValue={() => ContainerAPI.update(container)}
      edit={() => edit('status')}
      value={container.status}
      subValue={container.comments}
      editable={editable.status}
    >
      <ContainerField property={'status'}/>
      <ContainerField property={'comments'}/>
    </InlineField>
  );

  const projectField = () => !isEmpty(specimen) && (
    <InlineField
      label='Projects'
      clearAll={clearAll}
      updateValue={() => SpecimenAPI.update(specimen)}
      edit={() => edit('project')}
      value={specimen.projects.length !== 0 ?
       specimen.projects
         .map((id) => options.projects[id])
         .join(', ') : 'None'}
      editable={editable.project}
    >
      <SpecimenField property={'projects'}/>
    </InlineField>
  );

  const drawField = !isEmpty(specimen) && (
    <InlineField
      label='Draw Site'
      value={options.centers[
        options.sessionCenters[specimen.session].center
      ]}
    />
  );

  const centerField = (
    <InlineField
      label='Current Site'
      value={container.center}
    />
  );

  const shipmentField = () => {
    if (container.shipments.length !== 0) {
      return (
        <InlineField
          label='Shipment'
          value={container.shipments.slice(-1)[0]}
        />
      );
    }
  };

  const parentSpecimenField = () => {
    if (isEmpty(specimen)) {
      return null;
    }
  
    const value = specimen.parentSpecimens.length === 0
        ? 'None'
        : <>
            {specimen.parentSpecimens.map((barcode, index) => (
              <React.Fragment key={barcode}>
                {index > 0 && ', '}
                <Link to={'/specimens/'+barcode}>{barcode}</Link>
              </React.Fragment>
            ))}
          </>;

    return (
      <InlineField
        label="Parent Specimen"
        value={value}
      />
    );
  };

  // TODO: Find a way to make this conform to the GLOBAL ITEM structure.
  const parentContainerField = () => {
    if (loris.userHasPermission('biobank_container_view')) {
      // Set Parent Container Barcode Value if it exists
      const parentContainerBarcodeValue = () => {
        if (container.parentContainer) {
          const barcode = container.parentContainer
          // TODO: in the future, this should only be linked conditionally based
          // on whether the user has permission to view this container.
          return <Link to={'/containers/'+barcode}>{barcode}</Link>;
        }
      };

      const updateParentContainer = () => {
        if (loris.userHasPermission('biobank_container_update')) {
          return (
            <div>
              <div className='action' title='Move Container'>
                <span
                  className='action-button update'
                  onClick={() => {
                    edit('containerParentForm');
                  }}
                >
                  <span className='glyphicon glyphicon-chevron-right'/>
                </span>
              </div>
              <div>
                <Modal
                  title='Update Parent Container'
                  onClose={clearAll}
                  show={editable.containerParentForm}
                  onSubmit={() => ContainerAPI.update(container)}
                >
                  <ContainerParentForm
                    display={true}
                    container={container}
                  />
                </Modal>
              </div>
            </div>
          );
        }
      };

      let coordinate;
      if (container.coordinate) {
        coordinate = <CoordinateLabel container={container.getData()}/>;
      }

      return (
        <div className="item">
          <div className='field'>
            Parent Container
            <div className='value'>
              {parentContainerBarcodeValue() || 'None'}
            </div>
            {(parentContainerBarcodeValue && container.coordinate) ?
            'Coordinate '+ coordinate : null}
          </div>
          {updateParentContainer()}
        </div>
      );
    }
  };

  const candidateSessionField = !isEmpty(specimen) ? (
    <div>
      <InlineField
        label='PSCID'
        value={specimen.candidate}
        link={loris.BaseURL+'/'+specimen.candidate}
      />
      <InlineField
        label='Visit Label'
        value={specimen.session}
        link={
            loris.BaseURL+'/instrument_list/?candID='+
            specimen.candidate+'&sessionID='+
            specimen.session
        }
      />
    </div>
  ) : null;

  return (
    <div className="globals">
      <div className='list'>
        {!isEmpty(specimen) && (
          <InlineField label='Specimen Type'
            value={options.specimen.types[specimen.type]?.label}
          />
        )}
        {containerTypeField}
        {poolField}
        {quantityField}
        {fTCycleField()}
        {temperatureField}
        {statusField}
        {projectField()}
        {drawField}
        {centerField}
        {shipmentField()}
        {parentSpecimenField()}
        {parentContainerField()}
        {candidateSessionField}
      </div>
    </div>
  );
}

/**
 * Inline Field
 *
 * @param {object} props
 * @return {*}
 **/
const Item: React.FC = ({children}) => (
  <div className="item">{children}</div>
);

const InlineField: React.FC<{
  label: string,
  value: any,
  edit?: Function, //TODO: type declaration 
  editable?: boolean,
  pencil?: boolean,
  updateValue?: Function, //TODO: type declaration
  clearAll?: () => void,
  link?: string
  subValue?: string
}> = ({
  label,
  value,
  children,
  edit,
  editable = false,
  pencil = false,
  updateValue,
  clearAll,
  link,
  subValue,
}) => {
  const [loading, setLoading] = useState(false);

  // const fields = React.Children.map(children, (child) => {
  //   return child && React.isValidElement(child) ? (
  //     <div style={{flex: '1 0 25%', minWidth: '90px'}}>
  //       {React.cloneElement(child, {inputClass: 'col-lg-11'})}
  //     </div>
  //   ) : null;
  // });

  const handleUpdate = async () => {
    setLoading(true);
    try {
      await updateValue();
      edit();
      // Add any additional logic after the promise is resolved, if necessary
    } catch (error) {
      // Handle errors if necessary
    } finally {
      setLoading(false);
    }
  };


  // loris.userHasPermission('biobank_container_update') should determine if 'edit'
  // can be passed in the first place.
  const editButton = edit instanceof Function && !editable && (
    <div className='action' title={'Update '+label}>
      <span
        className={
            pencil
              ? 'glyphicon glyphicon-pencil'
              : 'action-button update'
        }
        onClick={() => edit()}
      >
        {!pencil && <span className='glyphicon glyphicon-chevron-right'/>}
      </span>
    </div>
  );

  const loader = loading && (
    <>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <Loader size={20}/>
      </div>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <h5 className='animate-flicker'>Saving...</h5>
      </div>
    </>
  );

  const submitButton = !loading && (
    <>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <Button label="Update" onClick={handleUpdate}/>
      </div>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <a onClick={clearAll} style={{cursor: 'pointer'}}>Cancel</a>
      </div>
    </>
  );

  const displayValue = link ? (
    <a href={link}>{value}</a>
  ) : value;

  const renderField = editable ? (
    <div className='field'>
      {label}
      <div className='inline-field'>
        {children}
        {submitButton}
        {loader}
      </div>
    </div>
  ) : (
    <div className="field">
      {label}
      {pencil && editButton}
      <div className='value'>
        {displayValue}
      </div>
      {subValue}
    </div>
  );

  return (
    <Item>
      {renderField}
      {!pencil && editButton}
    </Item>
  );
}

export default Globals;
