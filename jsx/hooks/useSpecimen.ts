import { Specimen, Process } from '../types';
import { useContainer, useEntity, useProcess } from '../hooks';

export function useSpecimen(
  initialSpecimen: Partial<Specimen> = {}
): Specimen {

  const validateSpecimen = (specimen: Specimen): Record<string, string> => {
    let errors: Record<string, string> = {};
    return errors;
  }

  const specimen = useEntity<Specimen>(initialSpecimen, validateSpecimen);

  const container = useContainer(specimen.container || {});
  const collection = useProcess(specimen.collection || {});
  const preparation = useProcess(specimen.preparation || {});
  const analysis = useProcess(specimen.analysis || {});

  // const url = `${loris.BaseURL}/biobank/specimens/${specimen.barcode}`;
  // const specimenPut = async (url: string): Promise<void> => {
  //   await put(url);
  // }

  return {
    ...specimen,
    container,
    collection,
    preparation,
    analysis,
  };
}
