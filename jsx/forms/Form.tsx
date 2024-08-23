import React, { useState, useEffect, ReactNode } from 'react';
import Utils from '../utils';
import Styles from '../styles';
import { useEntities, Entity, EntityHook } from '../entities';

interface MinMaxProps {
  min?: number;
  max?: number;
}

interface FieldProps {
  name: string;
  disabled?: boolean;
  placeholder?: string;
  autofocus?: boolean;
  error?: string;
}

interface ValueProps<T extends boolean | string | number> {
  value: T | null;
  onChange: (name: string, value: T | null) => void;
}

// Common properties for any inlined fields
interface LabelProps {
  label?: string;
  required?: boolean,
}

interface InlineFieldProps extends LabelProps {
  children: ReactNode
}

interface OptionsProps<T> {
  options: T[];
  format: (option: T) => string;
  emptyOption?: boolean;
}

// Properties specific to input elements, including handlers and common HTML attributes

interface ListProps<T> extends LabelProps, FieldProps, OptionsProps<T> {
  list: Map<string, T>;
  onAdd: (key: string, value: T) => void;
  onRemove: (key: string) => void;
}

interface TextProps
  extends ValueProps<string>, LabelProps, FieldProps{}

interface NumberProps
  extends ValueProps<number>, LabelProps, FieldProps, MinMaxProps{}

interface BooleanProps
  extends ValueProps<boolean>, LabelProps, FieldProps {}

interface SelectProps<T>
  extends TextProps, OptionsProps<T> {}

interface StaticProps
  extends LabelProps, Omit<ValueProps<any>, 'onChange'> {};

// Properties for button components, focusing on interaction
interface ButtonProps {
  onClick: (e: React.MouseEvent<HTMLButtonElement>) => void,
  label?: string;
  disabled?: boolean;
}

interface InputProps<T extends string | number | boolean> extends FieldProps, MinMaxProps, ValueProps<T> {
  type: 'date' | 'time' | 'text' | 'email' | 'number' | 'checkbox';
}

interface FormWrapperProps {
  onSubmit: (event: React.FormEvent<HTMLFormElement>) => void;
  children: ReactNode;
  submitLabel?: string;
  className?: string;
}

const FormWrapper: React.FC<FormWrapperProps> = ({
  onSubmit,
  children,
  submitLabel = 'Submit',
  className = '',
}) => {
  return (
    <form onSubmit={onSubmit} className={className}>
      {children}
      <div className="form-group mt-3">
        <Button
          onClick={(e) => {
            e.preventDefault();
            onSubmit(e as unknown as React.FormEvent<HTMLFormElement>);
          }}
          label={submitLabel}
        />
      </div>
    </form>
  );
};

export const Input = <T extends string | number | boolean>({
  type,
  name,
  placeholder,
  autofocus,
  min,
  max,
  disabled,
  error,
  value,
  onChange,
}: InputProps<T>) => {
  const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    const newValue = type === 'checkbox' ? event.target.checked : event.target.value;
    onChange(name, newValue as T);
  };  

  return (
    <>
      <input
        type={type}
        className='form-control'
        name={name}
        value={type !== 'checkbox' ? (value as string | number) : undefined}
        checked={type === 'checkbox' ? (value as boolean) : undefined}
        placeholder={placeholder}
        autoFocus={autofocus}
        min={min}
        max={max}
        disabled={disabled}
        onChange={handleChange}
      />
      {error && <span className="text-danger">{error}</span>}
    </>
  )
}

const Field = ({
  label,
  required,
  children,
}: InlineFieldProps): JSX.Element => {  

  return (
    <Styles.Layout.Field hasLabel={!!label}>
      {label && (
        <Styles.Layout.Row justify={'flex-end'}>
          <label className="control-label">
            {label}
            {required && <span className="text-danger">*</span>}
          </label>
        </Styles.Layout.Row>
      )}
      <Styles.Layout.Row>{children}</Styles.Layout.Row>
    </Styles.Layout.Field>
  );
};

export const DateField = ({
  required,
  label,
  ...props
}: TextProps): JSX.Element => {  

  const today = new Date().toISOString().split('T')[0];
  const setToday = () => props.onChange(props.name, today);

  return (
    <Field required={required} label={label}>
      <Input type='date' {...props}/>
      <Button onClick={setToday} label={'Today'}/>
    </Field>
  );
};

export const TimeField: React.FC<TextProps> = ({
  required,
  label,
  ...props
}) => {

  const handleNow = () => {
    const date = new Date();
    const hours = date.getHours();
    const minutes = date.getMinutes();
    const timeString = `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}`;
    props.onChange(props.name, timeString);
  };

  return (
    <Field required={required} label={label}>
      <Input type="time" {...props}/>
      <Button onClick={handleNow} label={'Now'}/>
    </Field>
  );
};

export const TextField: React.FC<TextProps> = ({
  required,
  label,
  ...props
}) => {
  return <Field required={required} label={label}><Input type='text' {...props}/></Field>;
};

export const NumberField: React.FC<NumberProps> = ({
  required,
  label,
  ...props
}) => {
  return (
    <Field required={required} label={label}>
      <Input type='number' {...props}/>
    </Field>
  );
};

export const StaticField: React.FC<StaticProps> = ({
  label,
  value,
}) => {
  return <Field label={label}>{value}</Field>;
};

export const CheckboxField: React.FC<BooleanProps> = ({
  required,
  label,
  ...props
}) => {
  return (
    <Field required={required} label={label}>
      <Input type='checkbox' {...props}/>
    </Field>
  );
};

export const TextareaField: React.FC<TextProps> = ({
  required,
  label,
  onChange,
  ...props
}) => {
  const handleChange = (event: React.ChangeEvent<HTMLTextAreaElement>) => {
    onChange(event.target.name, event.target.value);
  };

  return (
    <Field required={required} label={label}>
      <textarea className='form-control' onChange={handleChange} {...props} />
      {props.error && <span className="text-danger">{props.error}</span>}
    </Field>
  );
};

export const SelectField = <T,>({
  required,
  label,
  options,
  onChange,
  format,
  ...props
}: SelectProps<T>) => {
  const handleChange = (event: React.ChangeEvent<HTMLSelectElement>) => {
    onChange(event.target.name, options[event.target.value]);
  };

  const renderedOptions = Utils.normalizeOptions(options, format)

  return (
    <Field required={required} label={label}>
      <select className='form-control' onChange={handleChange} {...props}>
        <option value="">Select an option</option>
        {renderedOptions.map((value, key) => (
          <option key={key} value={key}>
            {value}
          </option>
        ))}
      </select>
      {props.error && <span className="text-danger">{props.error}</span>}
    </Field>
  );
};

export const SearchField = <T,>({
  required,
  label,
  options,
  onChange,
  format,
  ...props
}: SelectProps<T>) => {
  const [searchTerm, setSearchTerm] = useState('');
  const [filteredOptions, setFilteredOptions] = useState<string[]>([]);

  const renderedOptions = Utils.normalizeOptions(options, format);

  useEffect(() => {
    const filtered = Object.keys(options).filter(key =>
      options[key].toLowerCase().includes(searchTerm.toLowerCase())
    );
    setFilteredOptions(filtered);
  }, [searchTerm, options]);

  const handleChange = (name: string, value: string) => {
    setSearchTerm(value);
    const matchedOption = filteredOptions
    .find(option => option.toLowerCase() === searchTerm.toLowerCase());
    if (matchedOption) {
      handleOptionClick(matchedOption);
    } else {
      onChange(props.name, null);
    }
  };

  const handleOptionClick = (key: string) => {
    setSearchTerm(key);
    onChange(props.name, options[key]);
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


// Usage of InlineField and other components
export const ListField = <T extends {}>({
  label,
  onAdd,
  onRemove,
  list,
  ...props
}: ListProps<T>) => {

  const [input, setInput] = useState<string | null>(null);

  const handleSearch = (name: string, value: string) => {
    setInput(value);
  }

  const handleAdd = () => {
    if (input) {
      onAdd(input, props.options[input])
      setInput(null);
    }
  }

  return (
    <Styles.Form.ListContainer>
      <h4>{label + ' Input'}</h4>
      <Styles.Form.HorizontalRule />
      <SearchField
        value=''
        onChange={handleSearch}
        {...props}
      />
      <Button label='Add' onClick={handleAdd}/>
      <h4>{label + ' List'}</h4>
      <Styles.Form.HorizontalRule />
      {Array.from(list.keys()).map((key) => (
        <Styles.Form.ListItem>
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

export const Button: React.FC<ButtonProps> = ({
  label,
  onClick,
  disabled,
}) => {

  return (
    <button
      className={'btn btn-primary'}
      onClick={onClick}
      disabled={disabled}
    >
      {label}
    </button>
  );
}

const Form = {
 TextField,                                         
 DateField,                                         
 TimeField,                                         
 CheckboxField,                                  
 SelectField,                                     
 SearchField,                                     
 TextareaField,                                 
 ListField,                                        
 NumberField,                                     
 StaticField,        
 Button,
}

export default Form;

//                                                                                 
// /**                                                                             
//  * Input List                                                                   
//  *                                                                              
//  * @return {JSX}                                                                
//  */                                                                             
// function ListField({                                                            
//                      name,                                                      
//                      label,                                                     
//                      items,                                                     
//                      setItems,                                                  
//                      errorMessage,                                              
//                      options,                                                   
//                    }) {                                                         
//   const [item, setItem] = useState('');                                         
//                                                                                 
//   const removeItem = (index) => {                                               
//     setItems(name, items.filter((item, i) => index != i)); // XXX: added first param
//   };                                                                            
//   const addItem = () => {                                                       
//     const match = Object.keys(options)                                          
//       .find((key) => options[key][name] == item);                               
//     // if entry is in list of options and does not already exist in the list.   
//     if (match && !items.includes(match)) {                                      
//       setItems(name, [...items, match]); // XXX: added first param              
//       setItem(name, []); // XXX: added first param                              
//     }                                                                           
//   };                                                                            
//                                                                                 
//   const listStyle = {                                                           
//     border: '1px solid #DDD',                                                   
//     borderRadius: '10px',                                                       
//     minHeight: '85px',                                                          
//     padding: '5px',                                                             
//     marginBottom: '15px',                                                       
//     display: 'flex',                                                            
//     flexDirection: 'column',                                                    
//   };                                                                            
//                                                                                 
//   const listItemStyle = {                                                       
//     width: '100%',                                                              
//     display: 'flex',                                                            
//     alignItems: 'flex-start',                                                   
//     flexDirection: 'row',                                                       
//     justifyContent: 'space-between',                                            
//   };                                                                            
//                                                                                 
//   const itemsDisplay = items.map((item, i) => {                                 
//     const style = {                                                             
//       color: '#DDDDDD',                                                         
//       marginLeft: 10,                                                           
//       cursor: 'pointer',                                                        
//     };                                                                          
//     return (                                                                    
//       <div key={i} style={listItemStyle}>                                       
//         <div>{options[item][name]}</div>                                        
//         <div                                                                    
//           className='glyphicon glyphicon-remove'                                
//           onClick={() => removeItem(i)}                                         
//           style={style}                                                         
//         />                                                                      
//       </div>                                                                    
//     );                                                                          
//   });                                                                           
//                                                                                 
//   const error = errorMessage instanceof Array ?                                 
//     errorMessage.join(' ') : errorMessage;                                      
//   return (                                                                      
//     <div style={{display: 'flex', justifyContent: 'space-between'}}>            
//       <div style={{flex: '0.47'}}>                                              
//         <FormHeader header={label + ' Input'}/>                                 
//         <InlineField weights={[1, 0]}>                                          
//           <TextboxElement                                                       
//             name={name}                                                         
//             onUserInput={(name, value) => setItem(value)}                       
//             value={item}                                                        
//             errorMessage={error}                                                
//           />                                                                    
//           <Button                                                               
//             label='Add'                                                         
//             onClick={addItem}                                                   
//           />                                                                    
//         </InlineField>                                                          
//       </div>                                                                    
//       <div style={{flex: '0.47'}}>                                              
//         <FormHeader header={label + ' List'}/>                                  
//         <div style={listStyle}>                                                 
//           {itemsDisplay}                                                        
//         </div>                                                                  
//       </div>                                                                    
//     </div>                                                                      
//   );                                                                            
// }                                                                               
//                                                                                 
// /**                                                                             
//  * Inline Field                                                                 
//  *                                                                              
//  * @return {JSX}                                                                
//  */                                                                             
// function InlineField({children, label = '', weights = []}) {                    
//   const fields = React.Children.map(children, (child, i) => {                   
//     return (                                                                    
//       <div style={{flex: weights[i] || 0}}>                                     
//         {child}                                                                 
//       </div>                                                                    
//     );                                                                          
//   });                                                                           
//                                                                                 
//   const inlineStyle = {                                                         
//     display: 'flex',                                                            
//     flexFlow: 'row',                                                            
//     justifyContent: 'spaceBetween',                                             
//   };                                                                            
//   return (                                                                      
//     <div style={inlineStyle}>                                                   
//       {fields}                                                                  
//     </div>                                                                      
//   );                                                                            
// }                                                                               
