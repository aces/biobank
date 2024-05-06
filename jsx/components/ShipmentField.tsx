import { FieldConfiguration, User } from '../types';
import { Shipment, IShipment, ShipmentHook, Log, ILog, LogHook } from '../entities';
import { mapFormOptions } from '../utils';
import { DynamicField } from '../components';

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
    getOptions: (context) => {                                                  
      return context.options.shipment.types;                                           
    }                                                                           
  },                                                                            
  destinationCenter: {                                                                     
    label: 'Destination Center',                                                              
    type: 'select',                                                             
    required: true,                                                             
    getOptions: (context) => {                                                  
      return context.options.centers;                                           
    }                                                                           
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
    getOptions: (context) => {
      return Object.fromEntries(Object.values(context.options.users)
                                .map((user: User) => [user.label, user.label]));
    }
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
