import React from 'react';
import { FieldProps, LabelProps, ValueProps } from '../Form';

interface StaticProps extends ValueProps<any>, LabelProps {}

const StaticField: React.FC<StaticProps> = ({ label, value }) => {
  return <Field label={label}>{value}</Field>;
};

export default StaticField;

