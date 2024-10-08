import React, { ReactElement } from 'react';
import { FieldConfiguration } from '../types';
import { Entity, EntityHook } from '../entities';
import { useBiobankContext } from '../hooks';
import Utils from '../utils';
import Form from './Form';

const fieldComponentMapping: {
  [key: string]: React.ComponentType<any>;
} = {
  text: Form.TextField,
  date: Form.DateField,
  time: Form.TimeField,
  boolean: Form.CheckboxField,
  select: Form.SelectField,
  search: Form.SearchField,
  textarea: Form.TextareaField,
  input: Form.ListField,
  number: Form.NumberField,
  static: Form.StaticField,
};  

interface DynamicFieldProps<E extends Entity<I>, I extends object> {
  property: keyof I,
  hook: EntityHook<E, I>,
  field: FieldConfiguration<I, keyof I>,
  isStatic?: boolean,
};

// Define OptionType with required structure
interface OptionType {
  label: string;
  value: any; // Customize based on the actual value structure you expect
}

export const DynamicField = <E extends Entity<I>, I extends object>({
  property,
  hook,
  field,
  isStatic,
}: DynamicFieldProps<E, I>): ReactElement | null => { 
  const context = useBiobankContext();

  if (!field) return null; // might not be necessary1

  const fieldType = isStatic || hook.locked ? 'static' : field.type;
  const Field = fieldComponentMapping[fieldType];

  if (!Field) return null; // might not be necessary

  const value = hook.getData()[property] as any;

  const commonProps = {
    name: property as string,
    label: field.label,
    value: value,
    required: field.required || false,
    error: hook.errors[property] || '',
    disabled: field.disabled || false,
    onChange: (name: string, value: any) => hook.set(name as keyof I, value),
  };

  let additionalProps = {};

  // Check if the field type requires handling `select`, `search`, etc.
  if (['select', 'search', 'input'].includes(field.type)) {
    additionalProps = {
      options: field.getOptions(context),
      value: value,
      placeholder: field.placeholder || 'Search for a ' + field.label,
      emptyOption: field.emptyOption || false,
    };
  }

  return <Field {...commonProps} {...additionalProps} />;
};
