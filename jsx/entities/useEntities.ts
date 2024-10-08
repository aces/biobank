import { useCallback, useState } from 'react';
import { Entity } from './';
import { v4 as uuidv4 } from 'uuid'; // Import UUID generator

export type EntitiesHook<E extends Entity<I>, I extends object> = {                                  
  entities: Map<string, E>,                                             
  add: (entityData: Partial<I>) => string,                                          
  update: (key: string, entity: Partial<I>) => void,                            
  remove: (key: string) => void,                                                
  setAll: (property: keyof I, value: any) => void,                              
  saveAll: (saveFunc: (data: Partial<I>[]) => Promise<any>) => Promise<any>,
  validateAll: () => boolean,                                                   
  toArray: () => E[],                                                   
  getData: () => Partial<I>[],
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

  const toArray = useCallback(() => Array.from(entities.values()), [entities]);

  const getData = useCallback(() => toArray()
                              .map(entity => entity.getData()), [toArray]);

  const keys = useCallback(() => Array.from(entities.keys()), [entities]);

  const isEmpty = useCallback(() => entities.size === 0, [entities]);

  const add = useCallback((entityData: Partial<I>) => {
    const id = uuidv4();
    const entity = new entityConstructor(entityData);
    setEntities(prev => new Map(prev).set(id, entity));
    return id;
  }, [entityConstructor]);
                                                                                
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

  // const update = (id: string, updatedEntity: Entity<I>) => {
  //   setEntities((prevEntities) => {
  //     const newEntities = new Map(prevEntities);
  //     newEntities.set(id, updatedEntity);
  //     return newEntities;
  //   });
  // };
                                                                                
  const remove = useCallback((key: string) => {                                 
    setEntities(prev => {                                               
      // Clone the map and remove the entity by id                              
      const newEntities = new Map(prev);                                
      newEntities.delete(key);                                                  
      return newEntities;                                                       
    });                                                                         
  }, []);                                                                       
                                                                                
  const setAll = useCallback((property: keyof I, value: any) => {
    const updatedEntities = new Map(
      Array.from(entities.entries()).map(([key, entity]) => {
        const updatedEntity = entity.set(property, value); // Update the entity
        return [key, updatedEntity]; // Return the updated entity
      })
    );
    setEntities(updatedEntities); // Update the state
    return updatedEntities; // Return updated entities so you can use them immediately
  }, [entities]);

  const validateAll = useCallback(() => {
    const updatedEntities = new Map(entities); // Clone the entities map
    let allValid = true;
  
    updatedEntities.forEach((entity, id) => {
      const errors = entity.validate(); // Call the entity's validate method
      const isValid = Object.keys(errors).length === 0;
  
      if (!isValid) {
        allValid = false; // Set to false if any entity has errors
      }
  
      // Update the entity with errors and store it in the map
      updatedEntities.set(id, entity.setErrors(errors));
    });

    setEntities(updatedEntities); // Update the state only once after all iterations

    return allValid;
  }, [entities]);  

  const saveAll = useCallback(async (saveFunc: (data: Partial<I>[]) => Promise<any>) => {
    try {
      const response = await saveFunc(getData());

      console.log(response);
  
      if (response.status === 'error' && Array.isArray(response.errors)) {
        // Handle validation errors
        const updatedEntities = new Map(entities);
        const entitiesArray = Array.from(entities.entries());

        entitiesArray.forEach(([id, entity], index) => {
          const errors = response.errors[index] || {};
          const formattedErrors: Partial<Record<keyof I, string>> = {};
        
          // Iterate over each field in the errors object
          Object.keys(errors).forEach((field) => {
            // Concatenate the array of error messages into a single string
            const messagesArray = errors[field];
            formattedErrors[field as keyof I] = messagesArray.join(' ');
          });
        
          // Set the concatenated errors on the entity
          updatedEntities.set(id, entity.setErrors(formattedErrors));
        });        
  
        setEntities(updatedEntities);
  
        // Throw an error to prevent the modal from closing
        throw new Error('Validation errors occurred');
      } else {
        // Successful response
        return response;
      }
    } catch (error) {
      // Re-throw the error to be handled by the modal or higher-level error handlers
      throw error;
    }
  }, [entities, getData, setEntities]);
                                                                                
  return {
    entities,
    add,
    update,
    remove,
    setAll,
    validateAll,
    saveAll,
    toArray,
    getData,
    keys,
    isEmpty
  };       
};    
