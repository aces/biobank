import { useState, useCallback } from 'react';
import { Entity } from './';

export type EntityHook<E extends Entity<I>, I extends object> = Partial<I> & {                       
  getEntity: () => E,
  set: <K extends keyof I>(key: keyof I, value: I[K]) => void,                  
  remove: (key: keyof I) => void,                                               
  clear: () => void,                                                            
  reset: () => void,                                                            
  replace: (entityData: Partial<I>) => void,
  validate: () => void,                                                        
  errors: Partial<Record<keyof I, string>>,                                  
  getData: () => Partial<I>,
  setLocked: (locked: boolean) => void,
  locked: boolean,
}   

export function useEntity<E extends Entity<I>, I extends object>(                              
  initEntity: E,
): EntityHook<E, I> {                                                
  const [entity, setEntity] = useState(initEntity);                          
  const [errors, setErrors] = useState(initEntity.getErrors());
  const [locked, setLocked] = useState(false);

  const set = useCallback(<K extends keyof I>(key: keyof I, value: I[K]) => {
    setEntity(entity.set(key, value));
  }, [entity]);
  
  const remove = useCallback((key: keyof I) => {
    setEntity(entity.remove(key));
  }, [entity]);
  
  const clear = useCallback(() => {
    setEntity(entity.clear());
  }, [entity]);
  
  const reset = useCallback(() => {
    setEntity(entity.reset());
  }, [entity]);

  const replace = useCallback((entityData: Partial<I>) => {
    setEntity(entity.replace(entityData));
  }, [entity]);
  
  const validate = useCallback(() => {
    const newErrors = entity.validate();
    setEntity(entity.setErrors(newErrors));
    setErrors(newErrors);
  }, [entity]);

  const getData = () => {
    return entity.getData();
  }

  const getEntity = () => {
    return entity;
  }

  return {
    ...getData(),
    getEntity,
    set,
    remove,
    clear,
    reset,
    replace,
    validate,
    errors,             
    getData,
    setLocked,
    locked,
  }
}
