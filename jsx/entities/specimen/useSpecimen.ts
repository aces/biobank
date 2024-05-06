import { useEffect, useContext } from 'react';
import {
  Specimen,
  Process,
  Data,
  ISpecimen,
  IProcess,
  IData,
  SpecimenContext
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
                                                                                
export const useSpecimenContext = (): SpecimenHook => {                         
  const context = useContext(SpecimenContext);                                  
                                                                                
  if (context === undefined) {                                                  
    console.error('useSpecimenContext must be used within a BiobankProvider');   
    throw new Error('useSpecimenContext must be used within a BiobankProvider'); 
  }                                                                             
                                                                                
  return context;                                                               
};  

export function useSpecimen(
  initialSpecimen: Partial<ISpecimen> = {}
): SpecimenHook {

  const specimen = useEntity<Specimen, ISpecimen>(() => new Specimen(initialSpecimen));

  const container = useContainer(specimen.container);
  const collection = useProcess(specimen.collection);
  const preparation = useProcess(specimen.preparation);
  const analysis = useProcess(specimen.analysis);

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
  initialProcess: Partial<IProcess> = {}
): ProcessHook {

  const process = useEntity<Process, IProcess>(() => new Process(initialProcess));
  const data = useData(() => new Data(process.data));

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
  initialData: Partial<IData> = {}
): DataHook {

  return useEntity<Data, IData>(() => new Data(initialData));
}
