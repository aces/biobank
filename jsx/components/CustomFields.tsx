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

type CustomFieldsProps = {
  fields: {
    [key: string]: {
      label: string,
      required: boolean,
      datatypeId: string,
    },
  },
  data: any //TODO: type,
  setData: (attribute: string, value: any) => void, // Replace 'any' with a more specific type if possible
  errors: any //TODO: type,
};

/**
 * Biobank Custom Attribute Fields
 * 
 * @param {CustomFieldsProps} props - The props for the component.
 * @returns {ReactElement} JSX Element corresponding to each field.
 */
const CustomFields = ({
  errors,
  fields,
  data,
  setData,
}: CustomFieldsProps): ReactElement => {
  const { options } = useBiobankContext();

  return (
    <>
      {Object.keys(fields).map((attribute, key) => {
        const datatype = options.specimen.attributeDatatypes[
            fields[attribute]['datatypeId']
          ].datatype;
        if (datatype === 'text' || datatype === 'number') {
          return (
            <TextboxElement
              key={key}
              name={attribute}
              label={fields[attribute].label}
              onUserInput={setData}
              required={fields[attribute].required}
              value={data[attribute]}
              errorMessage={errors[attribute]}
            />
          );
        }

        if (datatype === 'date') {
          return (
            <DateElement
              key={key}
              name={attribute}
              label={fields[attribute].label}
              onUserInput={setData}
              required={fields[attribute].required}
              value={data[attribute]}
              errorMessage={errors[attribute]}
            />
          );
        }

        if (datatype === 'time') {
          return (
            <TimeElement
              key={key}
              name={attribute}
              label={fields[attribute].label}
              onUserInput={setData}
              required={fields[attribute].required}
              value={data[attribute]}
              errorMessage={errors[attribute]}
            />
          );
        }

        if (datatype === 'boolean') {
          return (
            <CheckboxElement
              key={key}
              name={attribute}
              label={fields[attribute].label}
              onUserInput={setData}
              required={fields[attribute].required}
              value={data[attribute]}
              errorMessage={errors[attribute]}
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
