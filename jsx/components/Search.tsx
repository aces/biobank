import { useState, ReactElement } from 'react';
import { useNavigate } from 'react-router-dom';
import Modal from 'Modal';
import Form from 'Form';
const { FormElement, TextboxElement } = Form;

interface SearchProps {
  title: string,
  show: boolean,
  onClose: () => void,
  barcodes?: {[key: string]: string},
}

/**
 * Provides a modal window that can be used to search barcodes.
 *
 * @component
 * @param {SearchProps} props - The component's props.
 * @returns {ReactElement} The rendered component.
 */
export const Search = ({
  title,
  show,
  onClose,
  barcodes = {},
}: SearchProps): ReactElement  => {

  const [barcode, setBarcode] = useState<string | null>(null);
  const navigate = useNavigate();

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
      navigate(`/barcode=${value}`);
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
