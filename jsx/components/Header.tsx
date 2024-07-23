import { ReactElement } from 'react';
import Modal from 'Modal';
import { SpecimenForm, LifeCycle, ContainerField, BarcodePathDisplay } from '../components';
import Swal from 'sweetalert2';
import { useSpecimenContext, useContainerContext } from '../entities';
import { useBarcodePageContext, useBiobankContext } from '../hooks';
import { LabelAPI, ContainerAPI } from '../APIs';

declare const loris: any;

const Header: React.FC<{
  clearAll: () => void,
}> = ({ clearAll }) => {

  const { editable, edit } = useBarcodePageContext();
  const { options } = useBiobankContext();
  const specimen = useSpecimenContext();
  const container = useContainerContext();

  /**
   * Renders an action button for adding aliquots based on specimen and container status.
   *
   * @returns {ReactElement} Action button for adding aliquots.
   */
  const renderActionButton = (): ReactElement => {
    if (container.status === 'Available' && specimen.quantity > 0 && !specimen.pool) {
      return (
        <div className='action-button add' onClick={() => edit('aliquotForm')}>
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
        <>
          <div className='action' title='Make Aliquots'>
            {renderActionButton()}
          </div>
          <SpecimenForm
            title='Add Aliquots'
            parent={specimen}
            show={editable.aliquotForm}
            onClose={clearAll}
          />
        </>
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
            onClick={() => edit('lotForm')}
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
            onClick={() => edit('expirationForm')}
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
      onSubmit={() => ContainerAPI.update(container)}
      onSuccess={() => edit('lotForm')}
    >
      <ContainerField property={'lotNumber'}/>
    </Modal>
  );

  const expirationForm = (
    <Modal
      title='Edit Expiration Date'
      onClose={clearAll}
      show={editable.expirationForm}
      onSubmit={() => ContainerAPI.update(container)}
      onSuccess={() => edit('expirationForm')}
    >
      <ContainerField property={'expirationDate'}/>
    </Modal>
  );

  const barcodePathDisplay = <BarcodePathDisplay container={container.getData()}/>;

  /**
   * Prints the barcode of the container and its type.
   *
   * @function
   * @returns {void}
   */
  const printBarcode = (): void => {
    const label = [
      {
        barcode: container.barcode,
        type: specimen.type,
      },
    ];
    LabelAPI.create(label)
      .then(() => {
        // This block executes if the creation was successful
        Swal.fire('Print Barcode Number: ' + container.barcode);
      })
      .catch((error) => {
        // This block executes if there was an error during creation
        console.error('Error during label creation:', error);
        // Optionally show an error message to the user
        Swal.fire('Error', 'Failed to print barcode', 'error');
      });
  };

  /**
   * Handles the container checkout action.
   *
   * @function
   * @returns {void}
   */
  const checkoutContainer = (): void => {
      container.set('parent', null);
      container.set('coordinate', null);
      // TODO: this function has to be created!
      //contHandler.put();
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
        container.parent ? (
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
      <LifeCycle specimen={specimen}/>
    </div>
  );
};

export default Header;
