import React, { FC, ReactElement } from 'react';
import Modal from 'Modal';
import LifeCycle from './lifeCycle.js';
import SpecimenForm from './specimenForm.js';
import Swal from 'sweetalert2';
import { FormElement, TextboxElement, DateElement } from 'Form';

declare const loris: any;


// TODO: Replace 'any' and 'Function' with appropriate declarations
/*
 * Props for the Header Component
 */
interface HeaderProps {
  data: any;
  options: any;
  container: any;
  specimen: any;
  editable: any;
  current: any;
  updateContainer: Function;
  edit: Function;
  setSpecimen: Function;
  createSpecimens: Function;
  setContainer: Function;
  editContainer: Function;
  clearAll: Function;
  increaseCoordinate: Function;
  getParentContainerBarcodes: Function;
  getBarcodePathDisplay: Function;
  printLabel: Function;
}

/**
 * Header component for displaying specimen information and actions.
 *
 * @component
 * @param {HeaderProps} props - The properties passed to the component.
 * @returns {ReactElement} React element representing the header.
 */
const Header = ({ 
  data,
  options,
  container,
  specimen,
  editable,
  current,
  updateContainer,
  edit,
  setSpecimen,
  createSpecimens,
  setContainer,
  editContainer,
  clearAll,
  increaseCoordinate,
  getParentContainerBarcodes,
  getBarcodePathDisplay,
  printLabel,
}: HeaderProps): ReactElement => {
  const status = options.container.stati[container.statusId].label;

  /**
   * Renders an action button for adding aliquots based on specimen and container status.
   *
   * @function
   * @returns {ReactElement} Action button for adding aliquots.
   */
  const renderActionButton = (): ReactElement => {
    if (status === 'Available' && specimen.quantity > 0 && !specimen.poolId) {
      const openAliquotForm = () => edit('aliquotForm');
      return (
        <div className='action-button add' onClick={openAliquotForm}>
          +
        </div>
      );
    } else {
      return <div className='action-button disabled'>+</div>;
    }
  };

  /**
   * Generates the form for adding aliquots based on user permissions and specimen data.
   *
   * @function
   * @returns {ReactElement|null} Aliquot form or null if the user doesn't have permission.
   */
  const addAliquotForm = (): ReactElement => {
    if (specimen && loris.userHasPermission('biobank_specimen_create')) {
      return (
        <div>
          <div className='action' title='Make Aliquots'>
            {renderActionButton()}
          </div>
          <SpecimenForm
            title='Add Aliquots'
            parent={[{ specimen: specimen, container: container }]}
            options={options}
            data={data}
            current={current}
            increaseCoordinate={increaseCoordinate}
            show={editable.aliquotForm}
            onClose={clearAll}
            setSpecimen={setSpecimen}
            onSubmit={createSpecimens}
          />
        </div>
      );
    }
  };

  /**
   * Handles editing the lot number of the container.
   *
   * @component
   * @returns {ReactElement|null} Edit lot number button or null if the user doesn't have permission.
   */
  const AlterLotNumberButton = (): ReactElement => {
    if (loris.userHasPermission('biobank_specimen_alter')) {
      return (
        <div className='action' title='Alter Lot Number'>
          <span
            style={{ color: 'grey' }}
            className='glyphicon glyphicon-pencil'
            onClick={() => {
              edit('lotForm');
              editContainer(container);
            }}
          />
        </div>
      );
    }
  };

  /**
   * Handles editing the expiration date of the container.
   *
   * @component
   * @returns {ReactElement|null} Edit expiration date button or null if the user doesn't have permission.
   */
  const AlterExpirationDateButton = (): ReactElement => {
    if (loris.userHasPermission('biobank_specimen_alter')) {
      return (
        <div className='action' title='Alter Expiration Date'>
          <span
            style={{ color: 'grey' }}
            className='glyphicon glyphicon-pencil'
            onClick={() => {
              edit('expirationForm');
              editContainer(container);
            }}
          />
        </div>
      );
    }
  };

  const lotForm = (
    <Modal
      title='Edit Lot Number'
      onClose={clearAll}
      show={editable.lotForm}
      onSubmit={updateContainer}
    >
      <FormElement>
        <TextboxElement
          name='lotNumber'
          label='Lot Number'
          onUserInput={setContainer}
          value={current.container.lotNumber}
        />
      </FormElement>
    </Modal>
  );

  const expirationForm = (
    <Modal
      title='Edit Expiration Date'
      onClose={clearAll}
      show={editable.expirationForm}
      onSubmit={updateContainer}
    >
      <FormElement>
        <DateElement
          name='expirationDate'
          label='Expiration Date'
          onUserInput={setContainer}
          value={current.container.expirationDate}
        />
      </FormElement>
    </Modal>
  );

  const parentBarcodes = getParentContainerBarcodes(container);
  const barcodePathDisplay = getBarcodePathDisplay(parentBarcodes);

  /**
   * Prints the barcode of the container and its type.
   *
   * @function
   * @returns {void}
   */
  const printBarcode = (): void => {
    const labelParams = [
      {
        barcode: container.barcode,
        type: options.specimen.types[specimen.typeId].label,
      },
    ];
    printLabel(labelParams).then(() =>
      Swal.fire('Print Barcode Number: ' + container.barcode)
    );
  };

  /**
   * Handles the container checkout action.
   *
   * @function
   * @returns {void}
   */
  const checkoutContainer = (): void => {
    editContainer(container)
      .then(() => setContainer('parentContainerId', null))
      .then(() => setContainer('coordinate', null))
      .then(() => updateContainer());
  };

  return (
    <div className='specimen-header'>
      <div className='specimen-title'>
        <div className='barcode'>
          Barcode
          <div className='value'>
            <strong>{container.barcode}</strong>
          </div>
          <span className='barcodePath'>
            Address: {barcodePathDisplay} <br />
            Lot Number: {container.lotNumber} <AlterLotNumberButton />
            <br />
            Expiration Date: {container.expirationDate}
            <AlterExpirationDateButton />
          </span>
          {lotForm}
          {expirationForm}
        </div>
        <div className='action' title='Print Barcode'>
          <div className='action-button update' onClick={printBarcode}>
            <span className='glyphicon glyphicon-print' />
          </div>
        </div>
        {addAliquotForm()}
        {loris.userHasPermission('biobank_container_update') &&
        container.parentContainerId ? (
          <div className='action'>
            <div
              className='action-button update'
              title='Checkout Container'
              onClick={checkoutContainer}
            >
              <span className='glyphicon glyphicon-share' />
            </div>
          </div>
        ) : null}
      </div>
      <LifeCycle specimen={specimen} centers={options.centers} />
    </div>
  );
};

export default Header;
