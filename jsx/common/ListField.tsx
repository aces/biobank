import React, { useState } from 'react';
import { SearchField } from '../FormFields';
import { FieldProps, LabelProps, OptionsProps } from '../Form';
import { Button } from '../Button';
import Styles from '../styles';

interface ListProps<T> extends LabelProps, FieldProps, OptionsProps<T> {
  list: Map<string, T>;
  onAdd: (key: string, value: T) => void;
  onRemove: (key: string) => void;
}

const ListField = <T extends {}>({
  label,
  onAdd,
  onRemove,
  list,
  ...props
}: ListProps<T>) => {
  const [input, setInput] = useState<string | null>(null);

  const handleSearch = (name: string, value: string) => {
    setInput(value);
  };

  const handleAdd = () => {
    if (input) {
      onAdd(input, props.options[input]);
      setInput(null);
    }
  };

  return (
    <Styles.Form.ListContainer>
      <h4>{label + ' Input'}</h4>
      <Styles.Form.HorizontalRule />
      <SearchField
        value=''
        onChange={handleSearch}
        {...props}
      />
      <Button label='Add' onClick={handleAdd} />
      <h4>{label + ' List'}</h4>
      <Styles.Form.HorizontalRule />
      {Array.from(list.keys()).map((key) => (
        <Styles.Form.ListItem key={key}>
          <div>{list[key]}</div>
          <Styles.Form.Icon
            className="glyphicon glyphicon-remove"
            onClick={() => onRemove(key)}
          />
        </Styles.Form.ListItem>
      ))}
    </Styles.Form.ListContainer>
  );
};

export default ListField;
