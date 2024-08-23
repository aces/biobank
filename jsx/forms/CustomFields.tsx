import { ReactElement } from 'react';
import {
  TextField,
  DateField,
  TimeField,
  CheckboxField,
} from './';
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

  return (
    <>
      {attributes.map((attribute, key) => {
        const datatype = attribute.datatype;
        const label = attribute.label;
        if (datatype === 'text' || datatype === 'number') {
          return (
            <TextField
              key={key}
              name={String(attribute.id)}
              label={attribute.label}
              onChange={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              error={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'date') {
          return (
            <DateField
              key={key}
              name={String(attribute.id)}
              label={attribute.label}
              onChange={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              error={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'time') {
          return (
            <TimeField
              key={key}
              name={String(attribute.id)}
              label={attribute.label}
              onChange={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              error={data.errors[attribute.label]}
            />
          );
        }

        if (datatype === 'boolean') {
          return (
            <CheckboxField
              key={key}
              name={String(attribute.id)}
              label={attribute.label}
              onChange={data.set}
              required={attribute.required}
              value={data[attribute.label]}
              error={data.errors[attribute.label]}
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
