import React, { useState, useEffect } from 'react';
import { Container, BiobankContextType } from '../types';
import { useBiobankContext } from '../hooks';
import Form from 'Form';
const {
  TextboxElement,
  SelectElement,
  DateElement,
} = Form;

interface FieldConfiguration<T> {
    label: string,
    type: 'text' | 'select' | 'date',
    required: boolean,
    getOptions?: (context: BiobankContextType) => Promise<Array<{ value: string; label: string }>>,
}

// TODO: find a way to implement this again:
// const removeChildContainers = (object, id) => {
//   delete object[id];
//   for (let key in containers) {
//     if (id == containers[key].parentContainerId) {
//       object = removeChildContainers(object, key);
//     }
//   }
//   return object;
// };

// TODO: find a way to implement this again:
// Delete child containers from options if a container is being placed in a
// another container.
// if (container) {
//   containerBarcodesNonPrimary = removeChildContainers(
//     containerBarcodesNonPrimary,
//     container.id
//   );
// }
const specimenFieldConfig: Record<keyof Specimen, FieldConfiguration<Specimen>> = {
  quantity: {
    label: 'Quantity',
    type: 'text',
    required: true,
  },
  unitId: {
    label: 'Units',
    type: 'text',
    required: true,
    getOptions: (context, specimen) => {
      return specimen.typeId ? mapFormOptions(
        context.options.specimen.typeUnits[specimen.typeId], 'label'
      ) : {};
    }
  },
  fTCycle: {
    label: 'Freeze-Thaw Cycle',
    type: 'number',
    required: true,
  },
  temperature: {
    label: 'Temperature',
    type: 'number',
    required: true,
  },
  projectIds: {
    label: 'Projects',
    type: 'select',
    multiple: true,
    required: true,
    emptyOption: false,
    getOptions: (context) => context.options.projects,
  }
}

const containerFieldConfig: Record<keyof Container, FieldConfiguration<Container>> = {
  barcode: {
    label: 'Barcode',
    type: 'text',
    required: true,
  },
  center: {
    label: 'Site',
    type: 'select',
    required: true,
    getOptions: (context) => {
      return context.options.centers;
    }
  },
  parentContainerId: {
    label: 'Parent Container Barcode',
    type: 'search',
    required: false,
    getOptions: (context) => {
      return Object.values(containers)
      .reduce((result, container) => {
        const dimensions = context.options.container.dimensions[containers[
          container.id
        ].dimensionId];
        const capacity = dimensions.x * dimensions.y * dimensions.z;
        const available = capacity - container.childContainerIds.length;
        result[container.id] = container.barcode +
             ' (' +available + ' Available Spots)';
        return result;
      }, {});
    }
  },
  lotNumber: {
    label: 'Lot Number',
    type: 'text',
    required: false,
  },
  expirationDate: {
    label: 'Expiration Date',
    type: 'date',
    required: false,
  },
  statusId: {
    label: 'Status',
    type: 'select',
    required: true,
    getOptions: async () => {
      const response = await fetch('https://api.example.com/status-options');
      const statuses = await response.json();
      return statuses.map(status => ({ value: status.id, label: status.name }));
    },
  },
  typeId: {
    label: 'Container Type',
    type: 'select',
    required: true,
    getOptions: (context) => {
      return mapFormOptions(context.options.containerTypesNonPrimary, 'label');
    }
  }
  // Define other fields as needed
};

const processFieldConfig: Record<keyof Process, FieldConfiguration<Process>> = {
  protocolId: {
    label: 'Protocol',
    type: 'select',
    required: true,
    getOptions: (context) => {
      let specimenProtocols = {};
      Object.entries(context.options.specimen?.protocols as { [key: string]: Protocol }).forEach(([id, protocol]) => {
        // FIXME: I really don't like 'toLowerCase()' function, but it's the
        // only way I can get it to work at the moment.
        if (typeId == protocol.typeId &&
            context.options.specimen.processes[protocol.processId].label.toLowerCase() ==
            processStage) {
          specimenProtocols[id] = protocol.label;
        }
      });
      return specimenProtocols;
    }
  },
  quantity: {
    label: 'Quantity',
    type: 'text',
    required: true,
  },
  unitId: {
    label: 'Unit',
    type: 'select',
    required: true,
    getOptions: context => Object.keys(context.options.specimen.typeUnits[typeId]||{})
     .reduce((result, id) => {
       result[id] = options.specimen.typeUnits[typeId][id].label;
       return result;
     }, {});
  },
  examinerId: {
    label: 'Done By',
    type: 'text',
    required: true,
    getOptions: context => mapFormOptions(context.options.examiners, 'label'),
  },
  date: {
    label: 'Date',
    type: 'date',
    required: true,
  },
  time: {
    label: 'Time',
    type: 'time',
    required: true,
  },
  comments: {
    label: 'Comments',
    type: 'textarea',
    required: true,
  },
};


export const ProcessField: React.FC<{
  property: keyof Process,
  process: process,
}> = ({
  property,
  process
}) => {
  const field = processFieldConfig[property];
  return <DynamicField property={property} entity={process} field={field}/>
}

export const ContainerField: React.FC<{
  property: keyof Container,
  container: Container,
}> = ({
  property,
  container
}) => {
  const field = containerFieldConfig[property];
  return <DynamicField property={property} entity={container} field={field}/>
}

const fieldComponentMapping = {
  text: TextboxElement,
  date: DateElement,
  time: TimeElement,
  boolean: CheckboxElement,
  select: SelectElement,
  search: SearchableDropdown,
  textarea: TextareaElement,
  number: NumericElement,
};

interface DynamicFieldProps<T> {
  property: keyof T;
  entity: T;
  field: FieldConfiguration<T>;
}

export const DynamicField = <T>({
  property,
  entity,
  field,
  static: isStatic = false,
}: DynamicFieldProps<T>) => { 

  const [options, setOptions] = useState({});
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState('');
  const context = useBiobankContext();

  useEffect(() => {
    const fetchOptions = async () => {
      if (field.getOptions) {
        try {
          const fetchedOptions = await field.getOptions(context, entity);
          setOptions(fetchedOptions);
        } catch (error) {
          console.error("Failed to fetch options", error);
          setError("Failed to fetch options");
        }
      }
    };
    fetchOptions();
  }, [property, field, context]);

  // Decide rendering based on fieldConfig type
  if (!field) return null;

  const FieldComponent = fieldComponentMapping[field.type];

  const commonProps = {
    name: property,
    label: field.label,
    onUserInput: (name, value) => entity.set(name, value),
    required: field.required,
    value: entity[property] || '',
    errorMessage: entity.errors[property] || '',
  };

  // If the field type is 'select', add select-specific properties
  if (field.type === 'select') {
    commonProps.multiple = field.multiple;
    commonProps.emptyOption = field.emptyOption;
  }

  const typesWithOptions = ['select', 'search']; // Extend this array as needed
  const additionalProps = typesWithOptions.includes(field.type) ? { options } : {};

  return <FieldComponent {...commonProps} {...additionalProps} />
};
