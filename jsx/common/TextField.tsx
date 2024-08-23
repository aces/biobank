import React from 'react';
import { Input, FieldProps, ValueProps, LabelProps } from '../Form';

interface TextProps extends ValueProps<string>, LabelProps, FieldProps {}

const TextField: React.FC<TextProps> = ({ required, label, ...props }) => {
  return (
    <Field required={required} label={label}>
      <Input type='text' {...props} />
    </Field>
  );
};

export default TextField;
