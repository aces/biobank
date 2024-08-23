import { useState, ReactElement } from 'react';
import { useHistory } from 'react-router-dom';
import Modal from 'Modal';
import { TextField } from '../forms';

/**
 * Provides a modal window that can be used to search barcodes.
 *
 * @component
 * @param {SearchProps} props - The component's props.
 * @returns {ReactElement} The rendered component.
 */
export const Search: React.FC<{
  title: string,
  show: boolean,
  onClose: () => void,
  barcodes?: {[key: string]: string},
}> = ({
  title,
  show,
  onClose,
  barcodes = {},
}) => {

  const [barcode, setBarcode] = useState<string | null>(null);
  const history = useHistory();

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
      onSubmit={() => Promise.resolve()}
    >
      <TextField
        name="barcode"
        label="Barcode"
        value={barcode}
        onChange={onInput}
        placeholder="Please Scan or Type Barcode"
        autofocus={true}
      />
    </Modal>
  );
}
