import React, { useState, useEffect } from 'react';
import Form from 'Form';
const {
  TextboxElement,
} = Form;

interface FieldConfiguration {
    label: string,
    type: 'text' | 'select',
    required: boolean,
    fetchOptions?: () => Promise<Array<{ value: string; label: string }>>,
}

const fieldConfig: Record<keyof Container, FieldConfiguration> = {
  quantity: {
      label: 'Quantity',
      type: 'text',
      required: true,
  },
  // Define other fields as needed
};

export const SpecimenField: React.FC<{
  property: keyof Specimen,
  Specimen: Specimen,
}> = ({ property, container }) => {
  const [options, setOptions] = useState([]);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');

  useEffect(() => {
    const fetchOptions = fieldConfig[property]?.fetchOptions;
    if (fetchOptions) {
        setLoading(true);
        fetchOptions().then(setOptions).catch(setError).finally(() => setLoading(false));
    }
  }, [property]);

  // Decide rendering based on fieldConfig type
  const field = fieldConfig[property];
  if (!field) return null;

  return (
    <>
      {field.type === 'text' && (
       <TextboxElement
         name={property}
         label={field.label}
         onUserInput={container.set}
         required={field.required}
         value={specimen[property]}
         errorMessage={specimen.errors[property]}
       />
      )}
    </>
  );
};
