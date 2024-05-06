import { useEntity, EntityHook } from '../';
import { IContainer, Container } from './';

export interface ContainerHook extends EntityHook<Container, IContainer> {
}

export function useContainer(
  initContainer: Partial<IContainer> = {}
): ContainerHook {

  return useEntity<Container, IContainer>(() => new Container(initContainer));
}
