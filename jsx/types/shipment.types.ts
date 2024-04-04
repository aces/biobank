import { ShipmentHandler } from '../types';

export interface Shipment extends ShipmentHandler {
  id: string,
  barcode: string,
  type: string,
  status: string,
  active: boolean,
  originCenterId: string,
  destinationCenterId: string,
  logs: Log[],
  containerIds: string[],
  containerBarcodes: string[],
}

export interface Log {
  barcode: string;
  centerId: string;
  status: string;
  user: string;
  temperature?: number;
  date: string;
  time: string;
  comments?: string;
}
