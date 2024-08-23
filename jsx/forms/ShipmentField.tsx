import { FieldConfiguration } from '../types';
import { useShipmentContext, useLogContext, Shipment, IShipment, IUser, ShipmentHook, Log, ILog, LogHook } from '../entities';
import { BaseAPI, ShipmentAPI } from '../APIs';
import { DynamicField } from '../forms';
import { useRequest } from '../hooks';

type ShipmentFields = Pick<IShipment, 'barcode' | 'type' | 'containers' |
  'originCenter' | 'destinationCenter'>; 
type ShipmentFieldConfig = FieldConfiguration<IShipment, keyof ShipmentFields>;

const shipmentFieldConfig: Record<keyof ShipmentFields, ShipmentFieldConfig> = {
  barcode: {                                                                    
    label: 'Barcode',                                                           
    type: 'text',                                                               
    required: true,                                                             
  },                                                                            
  type: {                                                                     
    label: 'Container Type',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new ShipmentAPI().getTypes())
  },                                                                            
  containers: {                                                                     
    label: 'Containers',                                                              
    type: 'input',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new ShipmentAPI().getTypes())
  },                                                                            
  originCenter: {                                                                     
    label: 'Origin Center',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new BaseAPI('centers').get())   
  },                                                                            
  destinationCenter: {                                                                     
    label: 'Destination Center',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new BaseAPI('centers').get())   
  }                                                                            
};                                                                              
                                                                                
export const ShipmentField: React.FC<{                                         
  property: keyof IShipment,                                                    
  isStatic?: boolean
}> = ({                                                                         
  property,                                                                     
  isStatic,
}) => {                                                                         
  const shipment = useShipmentContext();                                           
  const field = logFieldConfig[property];                                 
  return (
    <DynamicField
      property={property}
      hook={shipment}
      field={field}
      isStatic={isStatic}
     />   
  );
};    


type LogFields = Pick<ILog, 'center' | 'status' | 'temperature' | 'date' | 'time' | 'user' | 'comments'>;
type LogFieldConfig = FieldConfiguration<ILog, keyof LogFields>;

const logFieldConfig: Record<keyof LogFields, LogFieldConfig> = {
  center: {                                                                     
    label: 'Center',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new BaseAPI('centers').get())   
  },                                                                            
  status: {                                                                     
    label: 'Status',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: () => useRequest(new BaseAPI('centers').getStatuses())   
  },                                                                            
  temperature: {                                                                    
    label: 'Temperature',                                                           
    type: 'text',                                                               
    required: true,                                                             
  },                                                                            
  date: {                                                                    
    label: 'Date',                                                           
    type: 'date',                                                               
    required: true,                                                             
  },                                                                            
  time: {                                                                    
    label: 'Time',                                                           
    type: 'time',                                                               
    required: true,                                                             
  },                                                                            
  user: {                                                                    
    label: 'Done By',                                                           
    type: 'select',                                                               
    required: true,                                                             
    getOptions: () => useRequest(new BaseAPI('users').get()),
  },                                                                            
  comments: {                                                                    
    label: 'Comments',                                                           
    type: 'textarea',                                                               
    required: true,                                                             
  },                                                                            
};                                                                              

export const LogField: React.FC<{                                         
  property: keyof ILog,                                                    
  isStatic?: boolean,
}> = ({                                                                         
  property,                                                                     
  isStatic,
}) => {                                                                         
  const log = useLogContext();
  const field = logFieldConfig[property];                                 
  return <DynamicField property={property} hook={log} field={field}
  isStatic={isStatic}/>   
}    
