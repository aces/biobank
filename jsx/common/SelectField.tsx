import React from 'react';
import { FieldProps, ValueProps, LabelProps, OptionsProps, normalizeOptions } from '../Form';

interface SelectProps<T> extends ValueProps<T>, LabelProps, FieldProps, OptionsProps<T> {}

const SelectField = ({
  required,
  label,
  options,
  onChange,
  ...props
}: SelectProps<string>) => {
  const handleChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
    console.log(event.target.value);
    onChange(event.target.name, event.target.value);
  };

  const renderedOptions = normalizeOptions(options);

  return (
    <Field required={required} label={label}>
      <select className='form-control' onChange={handleChange} {...props}>
        <option value="">Select an option</option>
        {Object.keys(renderedOptions).map(key => (
          <option key={key} value={key}>
            {renderedOptions[key]}
          </option>
        ))}
      </select>
      {props.error && <span className="text-danger">{props.error}</span>}
    </Field>
  );
};

export default SelectField;
