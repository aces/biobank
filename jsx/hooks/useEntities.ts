import { useCallback, useState } from 'react';
import { EntityHandler } from '../types';
import { v4 as uuidv4 } from 'uuid'; // Import UUID generator

const useEntities = <T extends EntityHandler>() => {
  const [entities, setEntities] = useState<Map<string, Partial<T>>>(new Map());

  const add = useCallback((entity: Partial<T>) => {
    const key = uuidv4(); // Generate a new unique key
    setEntities(prevEntities => new Map(prevEntities).set(key, entity));
    return key; // Return the generated key if needed
  }, []);

  const update = useCallback((key: string, entity: Partial<T>) => {
    setEntities(prevEntities => {
      const existingEntity = prevEntities.get(key);
      if (existingEntity) {
        const updatedEntity = { ...existingEntity, ...entity };
        return new Map(prevEntities).set(key, updatedEntity as T);
      }
      return prevEntities;
    });
  }, []);

  const remove = useCallback((key: string) => {
    setEntities(prevEntities => {
      // Clone the map and remove the entity by id
      const newEntities = new Map(prevEntities);
      newEntities.delete(key);
      return newEntities;
    });
  }, []);

  // New function to set a specific property for all entities
  const setAll = useCallback((property: keyof T, value: any) => {
    setEntities(prev => new Map(Array.from(prev.entries()).map(
      ([key, entity]) => [key, { ...entity, [property]: value }]
    )));
  }, []);

  const validateAll = useCallback((): boolean => {
    return Array.from(entities.values()).every(entity => {
      const validationErrors = entity.validate();
      return Object.keys(validationErrors).length === 0; // True if no errors, hence valid
    });
  }, [entities]);

  // New function to prepare entities for posting (convert Map to Array or other structure)
  const toArray = useCallback((): T[] => {
    return Array.from(entities.values());
  }, [entities]);

  return { entities, add, update, remove, setAll, validateAll, toArray };
};
