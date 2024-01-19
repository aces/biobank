import React, { useState, ReactElement, ReactNode, useContext } from 'react';
import { Link } from 'react-router-dom';
import { mapFormOptions, isEmpty } from '../utils';
import Modal from 'Modal';
import Loader from 'Loader';
import { ContainerParentForm } from '../components';
import {
  Button,
  TextboxElement,
  TextareaElement,
  SelectElement,
  NumericElement
} from 'Form';
import { Options, Container, Specimen, ContainerHandler, SpecimenHandler } from '../types';
import { BarcodePageContext } from '../contexts';
import { ContainerAPI, SpecimenAPI } from '../APIs';

declare const loris: any;

type GlobalsType = {
  options: Options,
  specimen: Specimen,
  specHandler: SpecimenHandler,
  container: Container,
  contHandler: ContainerHandler,
  clearAll: () => void,
  getCoordinateLabel: Function, //TODO: type declaration
};

/**                                                                             
 * Header component for displaying specimen information and actions.            
 *                                                                              
 * @param {GlobalsType} props - The properties passed to the component.         
 * @returns {ReactElement} React element representing the header.               
 */       
function Globals({
  options,
  specimen,
  specHandler,
  container,
  contHandler,
  clearAll,
  getCoordinateLabel,
}: GlobalsType): ReactElement {
  const { editable, edit } = useContext(BarcodePageContext);

  const specimenTypeField = !isEmpty(specimen) && (
    <InlineField
      label='Specimen Type'
      value={options.specimen.types[specimen.typeId]?.label}
    />
  );

  const editContainerType = loris.userHasPermission('biobank_specimen_alter')
    && specimen && (
    () => {
      edit('containerType');
    }
  );
  const containerTypes = mapFormOptions(
    options.container.typesPrimary,
    'label'
  );
  const containerTypeField = (
    <InlineField
      label={'Container Type'}
      updateValue={() => ContainerAPI.update(container)}
      clearAll={clearAll}
      pencil={true}
      value={options.container.types[container.typeId]?.label} //TODO remove '?'
      edit={editContainerType}
      editable={editable.containerType}
    >
      <SelectElement
        name='typeId'
        onUserInput={contHandler.set}
        options={containerTypes}
        value={container?.typeId}
        errorMessage={contHandler.errors.typeId}
      />
    </InlineField>
  );

  const poolField = (specimen||{}).poolId ? (
    <InlineField
      label='Pool'
      value={specimen.poolLabel}
    />
  ) : null;

  const units = !isEmpty(specimen) ? mapFormOptions(
    options.specimen.typeUnits[specimen.typeId], 'label'
  ) : null;

  const quantityField = !isEmpty(specimen) ? (
    <InlineField
      label='Quantity'
      clearAll={clearAll}
      updateValue={() => SpecimenAPI.update(specimen)}
      edit={() => edit('quantity')}
      value={Math.round(specimen.quantity * 100) / 100+
      ' '+options.specimen.units[specimen.unitId].label}
      editable={editable.quantity}
    >
      <TextboxElement
        name='quantity'
        onUserInput={specHandler.set}
        value={specimen.quantity}
        errorMessage={specHandler.errors.quantity}
      />
      <SelectElement
        name='unitId'
        options={units}
        onUserInput={specHandler.set}
        value={specimen.unitId}
        errorMessage={specHandler.errors.unitId}
      />
    </InlineField>
  ) : null;

  const fTCycleField = () => {
    if (!isEmpty(specimen)
        && options.specimen.types[specimen.typeId].freezeThaw == 1) {
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
          <NumericElement
            name='fTCycle'
            onUserInput={specHandler.set}
            value={specimen.fTCycle}
            errorMessage={specHandler.errors.fTCycle}
          />
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
      edit={!container.parentContainerId && editTemperature}
      value={container.temperature + '°'}
      editable={editable.temperature}
    >
      <TextboxElement
        name='temperature'
        onUserInput={contHandler.set}
        value={container.temperature}
        errorMessage={contHandler.errors.temperature}
      />
    </InlineField>
  );

  const stati = mapFormOptions(options.container.stati, 'label');
  const renderCommentsField = () => {
    if (stati[container.statusId] !== 'Discarded' &&
        stati[container.statusId] !== 'Dispensed' &&
        stati[container.statusId] !== 'Shipped') {
      return [];
    }
    return (
      <TextareaElement
        name='comments'
        onUserInput={contHandler.set}
        value={container.comments}
        required={true}
      />
    );
  };
  const statusField = (
    <InlineField
      label={'Status'}
      clearAll={clearAll}
      updateValue={() => ContainerAPI.update(container)}
      edit={() => edit('status')}
      value={options.container.stati[container.statusId].label}
      subValue={container.comments}
      editable={editable.status}
    >
      <SelectElement
        name='statusId'
        options={stati}
        onUserInput={contHandler.set}
        value={container.statusId}
        errorMessage={contHandler.errors.statusId}
      />
      {renderCommentsField()}
    </InlineField>
  );

  const projectField = () => !isEmpty(specimen) && (
    <InlineField
      label='Projects'
      clearAll={clearAll}
      updateValue={() => SpecimenAPI.update(specimen)}
      edit={() => edit('project')}
      value={specimen.projectIds.length !== 0 ?
       specimen.projectIds
         .map((id) => options.projects[id])
         .join(', ') : 'None'}
      editable={editable.project}
    >
      <SelectElement
        name='projectIds'
        options={options.projects}
        onUserInput={specHandler.set}
        multiple={true}
        emptyOption={false}
        value={specimen.projectIds}
        errorMessage={specHandler.errors.projectIds}
      />
    </InlineField>
  );

  const drawField = !isEmpty(specimen) && (
    <InlineField
      label='Draw Site'
      value={options.centers[
        options.sessionCenters[specimen.sessionId].centerId
      ]}
    />
  );

  const centerField = (
    <InlineField
      label='Current Site'
      value={options.centers[container.centerId]}
    />
  );

  const shipmentField = () => {
    if (container.shipmentBarcodes.length !== 0) {
      return (
        <InlineField
          label='Shipment'
          value={container.shipmentBarcodes.slice(-1)[0]}
        />
      );
    }
  };

  const parentSpecimenField = () => {
    if (isEmpty(specimen)) {
      return null;
    }
  
    const { parentSpecimenIds, parentSpecimenBarcodes } = specimen;
    const value = parentSpecimenIds.length === 0
        ? 'None'
        : <>
            {parentSpecimenBarcodes.map((barcode, index) => (
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
        if (container.parentContainerBarcode) {
          const barcode = container.parentContainerBarcode
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
                  onSubmit={() => ContainerAPI(container)}
                >
                  <ContainerParentForm
                    display={true}
                    container={container}
                    contHandler={contHandler}
                    options={options}
                  />
                </Modal>
              </div>
            </div>
          );
        }
      };

      let coordinate;
      if (container.coordinate) {
        coordinate = getCoordinateLabel(container);
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
        value={options.candidates[specimen.candidateId].pscid}
        link={loris.BaseURL+'/'+specimen.candidateId}
      />
      <InlineField
        label='Visit Label'
        value={options.sessions[specimen.sessionId].label}
        link={
            loris.BaseURL+'/instrument_list/?candID='+
            specimen.candidateId+'&sessionID='+
            specimen.sessionId
        }
      />
    </div>
  ) : null;

  return (
    <div className="globals">
      <div className='list'>
        {specimenTypeField}
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
function Item({
  children,
}): ReactElement {
  return <div className="item">{children}</div>;
}

type InlineFieldType = {
  label: string,
  value: any,
  children?: ReactNode,
  edit?: Function, //TODO: type declaration 
  editable?: boolean,
  pencil?: boolean,
  updateValue?: Function, //TODO: type declaration
  clearAll?: () => void,
  link?: string
  subValue?: string
};

/**
 * Inline Field
 *
 * @param {InlineFieldType} props
 * @return {ReactElement}
 **/
function InlineField({
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
}: InlineFieldType): ReactElement {
  const [loading, setLoading] = useState(false);

  const fields = React.Children.map(children, (child) => {
    return child && React.isValidElement(child) ? (
      <div style={{flex: '1 0 25%', minWidth: '90px'}}>
        {React.cloneElement(child, {inputClass: 'col-lg-11'})}
      </div>
    ) : null;
  });

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

  const loader = loading && (
    <React.Fragment>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <Loader size={20}/>
      </div>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <h5 className='animate-flicker'>Saving...</h5>
      </div>
    </React.Fragment>
  );

  const submitButton = !loading && (
    <React.Fragment>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <Button
          label="Update"
          onClick={handleUpdate}
        />
      </div>
      <div style={{flex: '0 1 15%', margin: '0 1%'}}>
        <a onClick={clearAll} style={{cursor: 'pointer'}}>
          Cancel
        </a>
      </div>
    </React.Fragment>
  );

  const displayValue = link ? (
    <a href={link}>{value}</a>
  ) : value;

  const renderField = editable ? (
    <div className='field'>
      {label}
      <div className='inline-field'>
        {fields}
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
