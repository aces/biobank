import React from 'react';
import { Input, FieldProps, ValueProps, LabelProps } from '../Form';

interface CheckboxProps extends ValueProps<boolean>, LabelProps, FieldProps {}

const CheckboxField: React.FC<CheckboxProps> = ({ required, label, ...props }) => {
  return (
    <Field required={required} label={label}>
      <Input type='checkbox' {...props} />
    </Field>
  );
};

export default CheckboxField;
