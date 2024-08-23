import React from 'react';
import { Input, FieldProps, ValueProps, LabelProps } from '../Form';
import { Button } from '../Button';

interface DateProps extends ValueProps<string>, LabelProps, FieldProps {}

const DateField: React.FC<DateProps> = ({ required, label, ...props }) => {
  const today = new Date().toISOString().split('T')[0];
  const setToday = () => props.onChange(props.name, today);

  return (
    <Field required={required} label={label}>
      <Input type='date' {...props} />
      <Button onClick={setToday} label={'Today'} />
    </Field>
  );
};

export default DateField;
