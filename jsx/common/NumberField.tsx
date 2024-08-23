import React from 'react';
import { Input, FieldProps, ValueProps, LabelProps, MinMaxProps } from '../Form';

interface NumberProps extends ValueProps<number>, LabelProps, FieldProps, MinMaxProps {}

const NumberField: React.FC<NumberProps> = ({ required, label, ...props }) => {
  return (
    <Field required={required} label={label}>
      <Input type='number' {...props} />
    </Field>
  );
};

export default NumberField;

