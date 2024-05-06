import { useState } from 'react';
import { Form } from '../styles/form';
import { Layout } from '../styles/layout';
import { Entity, EntitiesHook } from '../entities';

// Extends properties for inlined fields and input-related functionalities,
// specifically omitting 'type' to prevent type specification at this component level.
interface FieldProps extends InlineFieldProps, Omit<InputProps, 'type'> {
};

// Common properties for any inlined fields
interface InlineFieldProps {
  label?: string;
  required?: boolean,
}

// Properties specific to input elements, including handlers and common HTML attributes
interface InputProps {
  name: string;
  value: string | number;
  min?: number;
  max?: number;
  disabled?: boolean;
  error?: string;
  onUserInput: (name: string, value: string) => void;
  type: 'date' | 'time' | 'text' | 'email' | 'number';
}

// Properties for button components, focusing on interaction
interface ButtonProps {
  onClick: () => void,
  label?: string;
  disabled?: boolean;
}

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

export const Input: React.FC<InputProps> = ({
  type,
  name,
  value,
  min,
  max,
  disabled,
  onUserInput,
  error,
}) => {
  const handleChange = (event: React.ChangeEvent<HTMLInputElement>) => {
    onUserInput(name, event.target.value);
  };
  return (
    <>
      <input
        type={type}
        className='form-control'
        name={name}
        value={value}
        min={min}
        max={max}
        disabled={disabled}
        onChange={handleChange}
      />
      {error && <span className="text-danger">{error}</span>}
    </>
  )
}

const Field: React.FC<InlineFieldProps> = ({
  label,
  required,
  children,
}) => {

  return (
    <Layout.Field hasLabel={!!label}>
      {label && (
        <Layout.Row justify={'flex-end'}>
          <label className="control-label">
            {label}
            {required && <span className="text-danger">*</span>}
          </label>
        </Layout.Row>
      )}
      <Layout.Row>{children}</Layout.Row>
    </Layout.Field>
  );
};

export const DateField: React.FC<FieldProps> = ({ required, label, ...props }) => {

  const today = new Date().toISOString().split('T')[0];
  const setToday = () => props.onUserInput(props.name, today);

  return (
    <Field required={required} label={label}>
      <Input type='date' {...props}/>
      <Button onClick={setToday} label={'Today'}/>
    </Field>
  );
};

export const TimeField: React.FC<FieldProps> = ({
  required,
  label,
  ...props
}) => {

  const handleNow = () => {
    const date = new Date();
    const hours = date.getHours();
    const minutes = date.getMinutes();
    const timeString = `${hours.toString().padStart(2, '0')}:${minutes.toString().padStart(2, '0')}`;
    props.onUserInput(props.name, timeString);
  };

  return (
    <Field required={required} label={label}>
      <Input type="time" {...props}/>
      <Button onClick={handleNow} label={'Now'}/>
    </Field>
  );
};

export const TextField: React.FC<FieldProps> = ({
  required,
  label,
  ...props
}) => {
  return <Field required={required} label={label}><Input type='text' {...props}/></Field>;
};

export const NumberField: React.FC<FieldProps> = ({
  required,
  label,
  ...props
}) => {
  return <Field required={required} label={label}><Input type='number' {...props}/></Field>;
};

export const StaticField: React.FC<FieldProps> = ({
  label,
  value,
}) => {
  return <Field label={label}>{value}</Field>;
};


// Usage of InlineField and other components
export const InputList = <E extends Entity<I>, I extends object>({
  label,
  list,
  options,
  error,
}: {
  label: string,
  list: EntitiesHook<E, I>,
  options: Record<string, I>,
  error?: string,
}) => {
  const [input, setInput] = useState('');

  const addItem = () => {
    // Implementation remains similar
     const item = Object.keys(options).find((key) => key == input);                               

     // if entry is in list of options and does not already exist in the list.   
     if (item) {                                      
       list.update(item, options[item]);
       setInput(''); 
     }
  };

  return (
    <Form.ListContainer>
      <h4>{label + ' Input'}</h4>
      <Form.HorizontalRule />
      <TextField
        name={name}
        onUserInput={(name, value) => setInput(value)}
        value={input}
        error={error}
      />
      <Button label='Add' onClick={addItem}/>
      <h4>{label + ' List'}</h4>
      <Form.HorizontalRule />
      {list.keys().map((key) => (
        <Form.ListItem>
          <div>{key}</div>
          <Form.Icon className="glyphicon glyphicon-remove" onClick={() => list.remove(key)}/>
        </Form.ListItem>
      ))}
    </Form.ListContainer>
  );
};

//                                                                                 
// /**                                                                             
//  * Input List                                                                   
//  *                                                                              
//  * @return {JSX}                                                                
//  */                                                                             
// function InputList({                                                            
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
