import { useCallback, useState } from 'react';
import { Entity } from './';
import { v4 as uuidv4 } from 'uuid'; // Import UUID generator

export type EntitiesHook<E extends Entity<I>, I extends object> = {                                  
  entities: Map<string, E>,                                             
  add: (entity: Partial<I>) => string,                                          
  update: (key: string, entity: Partial<I>) => void,                            
  remove: (key: string) => void,                                                
  setAll: (property: keyof I, value: any) => void,                              
  validateAll: () => boolean,                                                   
  toArray: () => E[],                                                   
  keys: () => string[],
  isEmpty: () => boolean,                                                       
}         

export const useEntities = <E extends Entity<I>, I extends object>(
  entityConstructor: new (data: Partial<I>) => E,
  initialEntities: Map<string, E> = new Map(),
): EntitiesHook<E, I> => {

  // XXX: this may later be changed such that useEntities already expects a map
  // const entitiesMap = new Map<string, E>(
  //   Object.entries(initialEntities).map(([key, data]) => [key, new entityConstructor(data)])
  // );

  const [entities, setEntities] = useState(initialEntities);

  const add = useCallback((entityData: Partial<I>) => {
    const id = uuidv4();
    const entity = new entityConstructor(entityData);
    setEntities(prev => new Map(prev).set(id, entity));
    return id;
  }, [entityConstructor]);
                                                                                
//   const update = useCallback((id: string, entityData: Partial<I>) => {             
//     setEntities(prev => {                                               
//       const entity = prev.get(id);                             
//       if (entity) {                                                     
//         const updatedEntity = Object.entries(entityData).reduce((acc, [key, value]) => {
//           return acc.set(key as keyof I, value);
//         }, entity);
//         return new Map(prev).set(id, updatedEntity);              
//       }                                                                         
//       return prev;                                                      
//     });                                                                         
//   }, []);                                                                       


  const update = useCallback((id: string, entityData: Partial<I>) => {
    setEntities(prev => {
      const entity = prev.get(id);
      if (entity) {
        let updatedEntity = entity;
        // Update each property in entityData on the entity
        Object.entries(entityData).forEach(([key, value]) => {
          // Assume 'set' method exists and is properly typed.
          // You might need to adjust if 'set' does not directly accept Partial<I> types.
          updatedEntity = updatedEntity.set(key as keyof I, value as I[keyof I]);
        });

        // XXX: this was removed to keep 'clone' function as protected.
        // const updatedEntity = entity.clone({
        //   ...entity.getData(),
        //   ...entityData
        // });
        return new Map(prev).set(id, updatedEntity);
      }
      return prev;
    });
  }, []);
                                                                                
  const remove = useCallback((key: string) => {                                 
    setEntities(prev => {                                               
      // Clone the map and remove the entity by id                              
      const newEntities = new Map(prev);                                
      newEntities.delete(key);                                                  
      return newEntities;                                                       
    });                                                                         
  }, []);                                                                       
                                                                                
  // New function to set a specific property for all entities                   
  const setAll = useCallback((property: keyof I, value: any) => {               
    setEntities(prev => new Map(
      Array.from(prev.entries()).map(([key, entity]) => {
        const updatedEntity = entity.set(property, value);
        return [key, updatedEntity];
      })
    ));                                                                        
  }, []);                                                                       
                                                                                
  const validateAll = useCallback(() => {                              
    return Array.from(entities.values()).every(entity => {                      
      const errors = entity.validate();                               
      return Object.keys(errors).length === 0; // True if no errors, hence valid
    });                                                                         
  }, [entities]);                                                               
                                                                                
  const toArray = useCallback(() => Array.from(entities.values()), [entities]);

  const keys = useCallback(() => Array.from(entities.keys()), [entities]);

  const isEmpty = useCallback(() => entities.size === 0, [entities]);
                                                                                
  return { entities, add, update, remove, setAll, validateAll, toArray, keys, isEmpty };       
};    
