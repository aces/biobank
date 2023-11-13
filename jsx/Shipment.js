import React, { useState } from 'react';
import { get, post } from './helpers.js';

/**
 * Custom hook for managing shipment data.
 *
 * @param {Shipment} initShipment - Initial shipment data.
 * @returns {Object} - Shipment state and functions to modify it.
 */
export function UseShipment(initShipment = {}) {
  const [init, setInit] = useState(initShipment);
  const [shipment, setShipment] = useState(new Shipment(init));
  const [errors, setErrors] = useState(new Shipment({}));

  this.set = (name, value) => setShipment(shipment.set(name, value));
  this.setContainerIds = (value) => this.set('containerIds', value);
  this.addLog = (log) => this.setLogs(shipment.addLog(log));
  this.setLogs = (value) => this.set('logs', value);
  this.setLog = (name, value, index) =>
        this.setLogs(shipment.setLog(name, value, index));
  this.remove = (name) => setShipment(shipment.remove(name));
  this.clear = () => {
    setShipment(new Shipment(init));
    setErrors(new Shipment({}));
  };
  this.post = async () =>
    await post(
        shipment,
        `${loris.BaseURL}/biobank/shipments/`,
        'POST'
    ).catch((e) => Promise.reject(setErrors(new Shipment(e))));
  this.put = async () =>
    await post(
        shipment,
        `${loris.BaseURL}/biobank/shipments/`,
        'PUT'
    ).then((shipments) => {
      setInit(new Shipment(shipments[0]));
      setShipment(new Shipment(shipments[0]));
      return shipments;
    })
    .catch((e) => Promise.reject(setErrors(new Shipment(e))));
  this.getShipment = () => shipment;
  this.getErrors = () => errors;

  return this;
}

/**
 * Represents a shipment of containers.
 */
interface ShipmentData {
  id?: string;
  barcode?: string;
  type?: string;
  destinationCenterId?: number;
  logs?: LogData[];
  containerIds?: string[];
}

class Shipment {
  /**
   * @param {ShipmentData} data
   */
  constructor(data: ShipmentData = {}) {
    this.id = data.id || null;
    this.barcode = data.barcode || null;
    this.type = data.type || null;
    this.destinationCenterId = data.destinationCenterId || null;
    this.logs = data.logs ? data.logs.map(log => new Log(log)) : [];
    this.containerIds = data.containerIds || [];
  }

  /**
   * Sets name to value in this shipment
   *
   * @param {string} name - the key
   * @param {object} value - the value
   *
   * @return {Shipment}
   */
  set(name, value) {
    return new Shipment({...this, [name]: value});
  }

  /**
   * ?
   *
   * @param {object} name - ?
   *
   * @return {Shipment}
   */
  remove(name) {
    return new Shipment({name, ...this});
  }

  /**
   * Load a shipment from the server
   *
   * @param {string} id - the id of the shipment
   *
   * @return {Shipment}
   */
  async load(id) {
   const shipment = await get(`${loris.BaseURL}/biobank/shipments/${id}`);
   return new Shipment(shipment);
  }

  /**
   * Adds a new log to this shipment
   *
   * @param {object} log - the log values
   *
   * @return {array}
   */
  addLog(log) {
    return [...this.logs, new Log(log)];
  };

  /**
   * Sets the log at index i to name/value
   *
   * @param {string} name - the log name
   * @param {?} value - the log value
   * @param {int} index - the index
   *
   * @return {array}
   */
  setLog(name, value, index) {
    return this.logs.map((log, i) => {
      if (i !== index) {
        return log;
      }
      return new Log({...log, [name]: value});
    });
  };
}

/**
 * Represents a shipment log
 */
interface LogData {
  barcode?: string;
  centerId?: string;
  status?: string;
  user?: string;
  temperature?: number;
  date?: string;
  time?: string;
  comments?: string;
}

class Log {
  barcode: string | null;
  centerId: string | null;
  status: string | null;
  user: string | null;
  temperature: number | null;
  date: string | null;
  time: string | null;
  comments: string | null;

  /**
   * @param {LogData} data
   */
  constructor(data: LogData = {}) {
    this.barcode = data.barcode || null;
    this.centerId = data.centerId || null;
    this.status = data.status || null;
    this.user = data.user || null;
    this.temperature = data.temperature || null;
    this.date = data.date || null;
    this.time = data.time || null;
    this.comments = data.comments || null;
  }

  /**
   * Sets name to value in this shipment.
   * 
   * @param {string} name - The key.
   * @param {any} value - The value.
   * @returns {Log} - Updated shipment.
   */
  set(name: keyof LogData, value: string | number | null): Log {
    return new Log({ ...this, [name]: value });
  }
}

export default Shipment;
