import { useEffect } from 'react';
import {
  Specimen,
  Process,
  Data,
  ISpecimen,
  IProcess,
  IData,
} from './';
import { useEntity, EntityHook } from '../';
import {
  Container,
  useContainer,
  ContainerHook,
} from '../container';

export interface SpecimenHook extends EntityHook<Specimen, ISpecimen> {
  container: ContainerHook;
  collection: ProcessHook;
  preparation: ProcessHook;
  analysis: ProcessHook;
}

export function useSpecimen(
  initialSpecimen: Specimen
): SpecimenHook {

  const specimen = useEntity<Specimen, ISpecimen>(initialSpecimen);

  const container = useContainer(new Container(specimen.container));
  const collection = useProcess(new Process(specimen.collection));
  const preparation = useProcess(new Process(specimen.preparation));
  const analysis = useProcess(new Process(specimen.analysis));

  return {
    ...specimen,
    container,
    collection,
    preparation,
    analysis,
  };
}

export interface ProcessHook extends EntityHook<Process, IProcess> {
  data: DataHook
}

export function useProcess(
  initialProcess: Process
): ProcessHook {

  const process = useEntity<Process, IProcess>(initialProcess);
  const data = useData(new Data(process.data));

  // if the protocol changes, the data should be reset
  useEffect(() => {
     data.clear();
  }, [process.protocol]);

  return {
    ...process,
    data,
  };
}

export interface DataHook extends EntityHook<Data, IData> {
  // You might add additional methods or properties specific to Data entities
}

export function useData(
  initialData: Data
): DataHook {
  return useEntity<Data, IData>(initialData);
}
