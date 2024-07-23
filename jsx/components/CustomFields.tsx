import { ReactElement } from 'react';
import Form from 'Form';
const {
  FormElement,
  TextboxElement,
  DateElement,
  TimeElement,
  CheckboxElement,
  FileElement
} = Form;
import { useBiobankContext } from '../hooks'; 
import { IAttribute } from '../entities';

type CustomFieldsProps = {
  attributes: IAttribute[],
  data: any //TODO: type,
};

/**
 * Biobank Custom Attribute Fields
 * 
 * @param {CustomFieldsProps} props - The props for the component.
 * @returns {ReactElement} JSX Element corresponding to each field.
 */
const CustomFields = ({
  attributes,
  data,
}: CustomFieldsProps): ReactElement => {
  const { options } = useBiobankContext();

  return (
    <>
      {attributes.map((attribute, key) => {
        const datatype = attribute.datatype;
        const label = attribute.label;
        if (datatype === 'text' || datatype === 'number') {
          return (
            <TextboxElement
              key={key}
              name={attribute}
              label={attribute.label}
              onUserInput={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              errorMessage={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'date') {
          return (
            <DateElement
              key={key}
              name={attribute}
              label={attribute.label}
              onUserInput={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              errorMessage={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'time') {
          return (
            <TimeElement
              key={key}
              name={attribute}
              label={attribute.label}
              onUserInput={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              errorMessage={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'boolean') {
          return (
            <CheckboxElement
              key={key}
              name={attribute}
              label={attribute.label}
              onUserInput={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              errorMessage={data.errors[attribute.label]}
            />
          );
        }

        // Do not present the possibility of uploading if file is already set
        // File must instead be deleted or overwritten.
        // TODO: rethink file uploading to not use current or data.
        // if (datatype === 'file' && !data[attribute]) {
        //   return (
        //     <FileElement
        //       key={key}
        //       name={attribute}
        //       label={fields[attribute].label}
        //       onUserInput={setData}
        //       required={fields[attribute].required}
        //       value={current.files[data[attribute]]}
        //       errorMessage={errors[attribute]}
        //     />
        //   );
        // }
      })};
    </>
  );
}

export default CustomFields;
