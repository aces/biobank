import { FieldConfiguration } from '../types';
import { Shipment, IShipment, IUser, ShipmentHook, Log, ILog, LogHook } from '../entities';
import { BaseAPI, ShipmentAPI } from '../APIs';
import { mapFormOptions } from '../utils';
import { DynamicField } from '../components';
import { useRequest } from '../hooks';

type ShipmentFields = Pick<IShipment, 'barcode' | 'type' | 'destinationCenter'>; 
type ShipmentFieldConfig = FieldConfiguration<IShipment>;

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
    getOptions: (context) => useRequest(new ShipmentAPI().getTypes())
  },                                                                            
  destinationCenter: {                                                                     
    label: 'Destination Center',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: (context) => useRequest(new BaseAPI('centers'))   
  },                                                                            
};                                                                              
                                                                                
export const ShipmentField: React.FC<{                                         
  property: keyof IShipment,                                                    
  shipment: ShipmentHook,                                                         
}> = ({                                                                         
  property,                                                                     
  shipment                                                                     
}) => {                                                                         
  const field = logFieldConfig[property];                                 
  return <DynamicField property={property} hook={shipment} field={field}/>   
}    


type LogFields = Pick<ILog, 'temperature' | 'date' | 'time' | 'user' | 'comments'>;
type LogFieldConfig = FieldConfiguration<ILog>;

const logFieldConfig: Record<keyof LogFields, LogFieldConfig> = {
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
    getOptions: (context) => useRequest(new BaseAPI('users')),
  },                                                                            
  comments: {                                                                    
    label: 'Comments',                                                           
    type: 'textarea',                                                               
    required: true,                                                             
  },                                                                            
};                                                                              

export const LogField: React.FC<{                                         
  property: keyof ILog,                                                    
  log: LogHook,                                                         
}> = ({                                                                         
  property,                                                                     
  log                                                                     
}) => {                                                                         
  const field = logFieldConfig[property];                                 
  return <DynamicField property={property} hook={log} field={field}/>   
}    
