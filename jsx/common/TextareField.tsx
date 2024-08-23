import React from 'react';
import { FieldProps, ValueProps, LabelProps } from '../Form';

interface TextareaProps extends ValueProps<string>, LabelProps, FieldProps {}

const TextareaField: React.FC<TextareaProps> = ({
  required,
  label,
  onChange,
  ...props
}) => {
  const handleChange = (event: React.ChangeEvent<HTMLTextAreaElement>) => {
    onChange(event.target.name, event.target.value);
  };

  return (
    <Field required={required} label={label}>
      <textarea className='form-control' onChange={handleChange} {...props} />
      {props.error && <span className="text-danger">{props.error}</span>}
    </Field>
  );
};

export default TextareaField;
