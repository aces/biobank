export interface Shipment {
  id?: string;
  barcode?: string;
  type?: string;
  destinationCenterId?: number;
  logs?: Log[];
  containerIds?: string[];
}

export interface Log {
  barcode?: string;
  centerId?: string;
  status?: string;
  user?: string;
  temperature?: number;
  date?: string;
  time?: string;
  comments?: string;
}
