import React, { ReactElement, useState, useEffect } from 'react';
import { FieldConfiguration } from '../types';
import { Entity, EntityHook } from '../entities';
import { useBiobankContext } from '../hooks';
import Form from 'Form';
import { DateField, TimeField, TextField, StaticField } from './Form';

const fieldComponentMapping = {
  text: TextField,
  date: DateField,
  time: TimeField,
  boolean: Form.CheckboxElement,
  select: Form.SelectElement,
  search: Form.SearchableDropdown,
  textarea: Form.Textarea,
  number: Form.NumericElement,
  static: StaticField,
};

interface DynamicFieldProps<E extends Entity<I>, I extends object> {
  property: keyof I,
  hook: EntityHook<E, I>,
  field: FieldConfiguration<I>,
  isStatic?: boolean,
};

export const DynamicField = <E extends Entity<I>, I extends object>({
  property,
  hook,
  field,
  isStatic,
}: DynamicFieldProps<E, I>): ReactElement | null => { 

  const [options, setOptions] = useState({});
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const context = useBiobankContext();

  useEffect(() => {
    const fetchOptions = async () => {
      if (field.getOptions) {
        try {
          const fetchedOptions = await field.getOptions(context);
          setOptions(fetchedOptions);
        } catch (error) {
          console.error("Failed to fetch options", error);
          setError("Failed to fetch options");
        }
      }
    };
    fetchOptions();
  }, [property, field, context]);

  // Decide rendering based on fieldConfig type
  if (!field) return null;

  const fieldType = isStatic ? 'static' : field.type
  const Field = fieldComponentMapping[fieldType];

  const commonProps = {
    name: property,
    label: field.label,
    onUserInput: (name, value) => hook.set(name, value),
    required: field.required || false,
    value: hook[property] || '',
    error: hook.errors[property] || '',
    disabled: field.disabled || false,
  };

  const typesWithOptions = ['select', 'search']; // Extend this array as needed
  const additionalProps = typesWithOptions.includes(field.type) ? {
    options,
    multiple: field.multiple,
    placeHolder: field.placeHolder || 'Search for a ' + field.label,
    emptyOption: field.emptyOption || false,
  } : {};

  return <Field {...commonProps} {...additionalProps}/>
};

