import React from 'react';
import { Input, FieldProps, ValueProps, LabelProps } from '../Form';
import { Button } from '../Button';

interface TimeProps extends ValueProps<string>, LabelProps, FieldProps {}

const TimeField: React.FC<TimeProps> = ({ required, label, ...props }) => {
  const handleNow = () => {
    const date = new Date();
    const hours = date.getHours();
    const minutes = date.getMinutes();
    const timeString = `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}`;
    props.onChange(props.name, timeString);
  };

  return (
    <Field required={required} label={label}>
      <Input type="time" {...props} />
      <Button onClick={handleNow} label={'Now'} />
    </Field>
  );
};

export default TimeField;
