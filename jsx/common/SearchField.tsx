import React, { useState, useEffect } from 'react';
import { Input, FieldProps, ValueProps, LabelProps, OptionsProps, normalizeOptions } from '../Form';

interface SearchProps<T> extends ValueProps<T>, LabelProps, FieldProps, OptionsProps<T> {}

const SearchField = <T,>({
  required,
  label,
  options,
  onChange,
  ...props
}: SearchProps<T>) => {
  const [searchTerm, setSearchTerm] = useState('');
  const [filteredOptions, setFilteredOptions] = useState<string[]>([]);

  const renderedOptions = normalizeOptions(options);

  useEffect(() => {
    const filtered = Object.keys(options).filter(key =>
      options[key].toLowerCase().includes(searchTerm.toLowerCase())
    );
    setFilteredOptions(filtered);
  }, [searchTerm, options]);

  const handleChange = (name: string, value: string) => {
    setSearchTerm(value);
    const matchedOption = filteredOptions.find(option => option.toLowerCase() === searchTerm.toLowerCase());
    if (matchedOption) {
      handleOptionClick(matchedOption);
    } else {
      onChange(props.name, null);
    }
  };

  const handleOptionClick = (key: string) => {
    setSearchTerm(key);
    onChange(props.name, key);
  };

  return (
    <Field required={required} label={label}>
      <Input
        type='text'
        value={searchTerm}
        onChange={handleChange}
        {...props}
      />
      <ul>
        {filteredOptions.map(key => (
          <li key={key} onClick={() => handleOptionClick(key)}>
            {renderedOptions[key]}
          </li>
        ))}
      </ul>
    </Field>
  );
};

export default SearchField;

