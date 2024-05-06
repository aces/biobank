import { IShipment, ILog } from './';                                                      
import { Entity } from '../';                                                      
                                                                                
export class Shipment extends Entity<IShipment> {                                         
  constructor(initialData: Partial<IShipment>) {                                              
    super(initialData);                                                         
  }                                                                             

  validate() {                                          
    const errors: Partial<Record<keyof IShipment, string>> = {};                                  

    return errors;                                                              
  }                                                                             

  // addLog(log: Partial<ILog> = {}): Partial<ILog>[] {
  //   return this.addToArray('logs', log);
  // }

  // setLog(index: number, updatedLog: Partial<ILog>): Partial<ILog>[] {
  //   return this.updateArray('logs', index, updatedLog);
  // }

  // removeLog(index: number): Partial<ILog>[] {
  //   return this.removeFromArray('logs', index);
  // }

  addLog(log: Partial<ILog> = {}): Partial<ILog>[] {
    return [...this.data.logs, log];
  }

  setLog(index: number, updatedLog: Partial<ILog>): Partial<ILog>[] {
    return this.data.logs.map((log, i) =>
      i === index ? updatedLog : log
    );
  }

  removeLog(index: number): Partial<ILog>[] {
    return this.data.logs.filter((_, i) => i !== index);
  }
}  

export class Log extends Entity<ILog> {
  validate() {
    const errors = {};                                  

    return errors;                                                              
  }
}

