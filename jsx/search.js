import React, { useState } from 'react';
import Modal from 'Modal';

/**
 * Provides a modal window that can be used to search barcodes.
 *
 * @component
 * @param {Object} props - The component's props.
 * @param {string} props.title - The title for the modal.
 * @param {boolean} props.show - A flag to control the visibility of the modal.
 * @param {Function} props.onClose - A callback function to close the modal.
 * @param {Object} props.barcodes - An object containing barcode data.
 * @param {Object} props.history - React Router history object for navigation.
 * @returns {JSX.Element} The rendered component.
 */
function Search({
  title,
  show,
  onClose,
  barcodes = {},
  history,
}) {
  /**
   * State for storing the barcode value.
   *
   * @type {string | null}
   */
  const [barcode, setBarcode] = useState(null);

  /**
   * Handles user input in the barcode textbox.
   *
   * @param {string} name - The name of the input element.
   * @param {string} value - The new value of the barcode.
   */
  const onInput = (name, value) => {
    setBarcode(value);
    if (Object.values(barcodes).find((barcode) => barcode === value)) {
      history.push(`/barcode=${value}`);
      onClose();
    }
  };

  return (
    <Modal
      title={title}
      show={show}
      onClose={onClose}
      throwWarning={false}
    >
      <FormElement>
        <TextboxElement
          name="barcode"
          label="Barcode"
          value={barcode}
          options={barcodes}
          onUserInput={onInput}
          placeHolder="Please Scan or Type Barcode"
          autoFocus={true}
        />
      </FormElement>
    </Modal>
  );
}

export default Search;
