import { Shipment, Log } from '../types';
import { useEntity, EntityHandler } from '../hooks';

// Overload signatures
export function useShipment(): Partial<Shipment>;
export function useShipment(initialShipment: Shipment): Shipment;

export function useShipment(
  initialShipment: Partial<Shipment> = {}
): Partial<Shipment> {

  const validateShipment = (shipment: Partial<Shipment>): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const shipment = useEntity<Partial<Shipment>>(initialShipment, validateShipment);

  const setLog = (
    name: keyof Log,
    value: any,
    index: number,
  ) => {
    shipment.set(
      'logs',
      shipment.logs.map((log, i) => i === index ? {...log, [name]: value} : log)
    );
  };

  // const url = `${loris.BaseURL}/biobank/specimens/${specimen.barcode}`;
  // const specimenPut = async (url: string): Promise<void> => {
  //   await put(url);
  // }

  return {
    ...shipment,
    setLog,
  };
}
