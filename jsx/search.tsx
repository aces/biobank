import React, { useState, ReactElement } from 'react';
import Modal from 'Modal';
import {FormElement, TextboxElement} from 'Form';

/**
 * Props for the Search component.
 */
interface SearchProps {
  title: string;
  show: boolean;
  onClose: () => void;
  barcodes?: {[key: string]: string};
  history: any; // TODO: Replace 'any' with appropriate type for history object
}

/**
 * Provides a modal window that can be used to search barcodes.
 *
 * @component
 * @param {SearchProps} props - The component's props.
 * @returns {ReactElement} The rendered component.
 */
const Search = ({
  title,
  show,
  onClose,
  barcodes = {},
  history,
}: SearchProps): ReactElement  => {
  /**
   * State for storing the barcode value.
   *
   * @type {string | null}
   */
  const [barcode, setBarcode] = useState<string | null>(null);

  /**
   * Handles user input in the barcode textbox.
   *
   * @function
   * @param {string} name - The name of the input element.
   * @param {string} value - The new value of the barcode.
   * @returns {void}
   */
  const onInput = (name: string, value: string) : void => {
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
