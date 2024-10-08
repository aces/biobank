import React, { useContext } from 'react';
import { BiobankContext, BiobankContextType } from '../contexts'; // Adjust the path as necessary

// Function to wrap the data array with Proxy for lazy loading
const wrapDataWithProxy = (dataArray, entityType, context) => {
  return dataArray.map((entity, index) => wrapEntityWithProxy(entity,
                                                              entityType,
  context, index ));
};

// Function to wrap individual entities with Proxy
const wrapEntityWithProxy = (entity, entityType, context, index) => {
  return new Proxy(entity, {
    get(target, prop, receiver) {
      let value = target[prop];

      // Lazy loading logic: hydrate the entity if a property is accessed and is undefined
      if (value === undefined && typeof prop === 'string' && context[entityType]) {
        const fullEntity = context[entityType].data.find(e => e.id === target.id);
        if (fullEntity) {
          context[entityType].updateEntity(index, fullEntity);
          // Update the target (current object) with the hydrated entity's properties
          Object.assign(target, fullEntity);
          value = target[prop];  // Re-attempt to access the property after hydration
        } else {
          console.warn(`${entityType} with ID ${target.id} not found in the context`);
          return undefined;
        }
      }

      // Recursively wrap nested objects with Proxy
      if (typeof value === 'object' && value !== null) {
        return wrapEntityWithProxy(value, entityType, context, index);
      }

      return value;
    }
  });
};

// Hook to use the BiobankContext and wrap streams for lazy loading
export const useBiobankContext = (): BiobankContextType => {
  const context = useContext(BiobankContext);

  if (!context) {
    console.error('useBiobankContext must be used within a BiobankProvider');
    throw new Error('useBiobankContext must be used within a BiobankProvider');
  }

  return context;

  // // Wrap streams' data with Proxy
  // const wrappedStreams = Object.keys(context).reduce((acc, key) => {
  //   if (context[key].data) {
  //     acc[key] = {
  //       ...context[key],
  //       data: wrapDataWithProxy(context[key].data, key, context), // Pass the context correctly here
  //     };
  //   } else {
  //     acc[key] = context[key];
  //   }
  //   return acc;
  // }, {} as BiobankContextType);

  // return wrappedStreams;
};

