import { SpecimenHandler, Specimen, Process } from '../types';
import { useGenericState, GenericStateHandler } from '../hooks';

function useSpecimen(
  initialSpecimen: Specimen = {}
): [Specimen, SpecimenHandler] {

  const validateSpecimen = (specimen: Specimen): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const [
    specimen,
    {
      set,
      remove,
      reset,
      validate,
      errors,
    }
  ] = useGenericState<Specimen>(initialSpecimen, validateSpecimen);

  const setProcess = (
    process: 'collection' | 'preparation' | 'analysis',
    name: keyof Process,
    value: any
  ) => {
    set(process, {
      ...specimen[process],
      [name]: value,
    });
  };

  const setData = (
    process: 'collection' | 'preparation' | 'analysis',
    key: string,
    value: any
  ) => {
    set(process, {
      ...specimen[process],
      data: {
        ...specimen[process].data,
        [key]: value,
      },
    });
  };

  // const url = `${loris.BaseURL}/biobank/specimens/${specimen.barcode}`;
  // const specimenPut = async (url: string): Promise<void> => {
  //   await put(url);
  // }

  return [
    specimen,
    {
      set,
      setProcess,
      setData,
      remove,
      reset,
      validate,
      errors
    }
  ];
}

export default useSpecimen;
