import { ShipmentHandler, Shipment, Log } from '../types';
import { useGenericState, GenericStateHandler } from '../hooks';

function useShipment(
  initialShipment: Shipment = {}
): [Shipment, ShipmentHandler] {

  const validateShipment = (shipment: Shipment): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const [
    shipment,
    {
      set,
      remove,
      reset,
      validate,
      errors,
    }
  ] = useGenericState<Shipment>(initialShipment, validateShipment);

  const setLog = (
    index: number,
    name: keyof Log,
    value: any
  ) => {
    set(
      'logs',
      shipment.logs.map((log, i) => i === index ? {...log, [name]: value} : log)
    );
  };

  // const url = `${loris.BaseURL}/biobank/specimens/${specimen.barcode}`;
  // const specimenPut = async (url: string): Promise<void> => {
  //   await put(url);
  // }

  return [
    shipment,
    {
      set,
      setLog,
      remove,
      reset,
      validate,
      errors
    }
  ];
}

export default useShipment;
