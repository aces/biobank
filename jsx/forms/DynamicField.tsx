import React, { ReactElement } from 'react';
import { FieldConfiguration } from '../types';
import { Entity, EntityHook } from '../entities';
import { useBiobankContext } from '../hooks';
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

export const DynamicField = <E extends Entity<I>, I extends object>({
  property,
  hook,
  field,
  isStatic,
}: DynamicFieldProps<E, I>): ReactElement | null => { 

  // const [options, setOptions] = useState<Record<string, any>>({});
  // const [loading, setLoading] = useState(false);
  // const [error, setError] = useState('');/
  const context = useBiobankContext();

 //  useEffect(() => {
 //    const fetchOptions = async () => {
 //      if (field.getOptions) {
 //        try {
 //          const fetchedOptions = await field.getOptions(context);
 //          setOptions(fetchedOptions);
 //        } catch (error) {
 //          console.error("Failed to fetch options", error);
 //          setError("Failed to fetch options");
 //        }
 //      }
 //    };
 //    fetchOptions();
 //  }, [property, field, context]);

  // Decide rendering based on fieldConfig type
  if (!field) return null;

  const fieldType = isStatic || hook.locked ? 'static' : field.type
  const Field = fieldComponentMapping[fieldType];

  if (!Field) return null;

  const commonProps = {
    name: property as string,
    label: field.label,
    value: hook.getData()[property] as any,
    required: field.required || false,
    error: hook.errors[property] || '',
    disabled: field.disabled || false,
    onChange: (name: string, value: any) => hook.set(name as keyof I, value),
  };

  let additionalProps = {};
  if (['select', 'search', 'input'].includes(field.type)) {
    additionalProps = {
      options: field.getOptions ? field.getOptions(context) : {},
      format: field.format || ((option) => (option as {label: string}).label), //current hack to allow this default, find better solution later
      //multiple: field.multiple,
      placeholder: field.placeholder || 'Search for a ' + field.label,
      emptyOption: field.emptyOption || false,
    };
  }

  return <Field {...commonProps} {...additionalProps}/>
};
