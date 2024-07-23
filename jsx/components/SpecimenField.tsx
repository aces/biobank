import React, { useState, useEffect } from 'react';
import { FieldConfiguration } from '../types';
import { ISpecimen, SpecimenHook, useSpecimenContext, IProcess, ProcessHook, IData, DataHook } from '../entities';
import { DynamicField } from '../components';
import { useRequest } from '../hooks';
import { SpecimenAPI, BaseAPI } from '../APIs';
import { mapFormOptions } from '../utils';

type SpecimenFields = Pick<ISpecimen, 'candidate' | 'session' | 'type' | 'quantity' | 'unit' | 'fTCycle' |
  'projects'>;
type SpecimenFieldConfig = FieldConfiguration<ISpecimen>;

const getSpecimenFieldConfig = (
  specimen: SpecimenHook
): Record<keyof SpecimenFields, SpecimenFieldConfig> => ({
  candidate: {
    label: "PSCID",
    type: 'search',
    required: true,
    placeHolder: 'Search for a PSCID',
    getOptions: (context) => {
        return useRequest(new BaseAPI('candidates'));
    },
  },
  session: {
    label: 'Visit Label',
    type: 'select',
    required: true,
    disabled: !specimen.candidate,
    // autoSelect: true,
    getOptions: (context) => {
      return specimen.candidate ?
        new BaseAPI('sessions').get({field: 'candidate', value:
                                    specimen.candidate.label})
      : {}
    },
  },
  type: {
    label: 'Specimen Type',
    type: 'select',
    required: true,
    getOptions: (context) => {
      // let specimenTypes;
      if (specimen.parents) {
        return useRequest(new SpecimenAPI().getTypes(
          specimen.parents[0].type.label
        ));
      }

      return new SpecimenAPI().getTypes();
    }
  },
  quantity: {
    label: 'Quantity',
    type: 'text',
    required: true,
  },
  unit: {
    label: 'Units',
    type: 'text',
    required: true,
    disabled: !specimen.type,
    getOptions: (context) => {
      return specimen.type ? specimen.type.units : {};
    }
  },
  fTCycle: {
    label: 'Freeze-Thaw Cycle',
    type: 'number',
    required: true,
  },
  projects: {
    label: 'Projects',
    type: 'select',
    multiple: true,
    required: true,
    disabled: !specimen.candidate,
    getOptions: (context) => new BaseAPI('projects').getAll(),
  },
})

// export const SpecimensField: React.FC<{
// }> = () => {
//   const specimens = useSpecimensContext();
//   const field = {
//     label: 'Search Specimen',
//     type: 'input',
//     getOptions: () => {
//       const barcodesPrimary = useMemo(() => {
//         return Object.values(contextSpecimens).reduce((result, specimen: ISpecimen) => {
//           if (!specimens.entities.has(specimen.id)) {
//             result[specimen.barcode] = specimen.barcode;
//           }
//           return result;
//         }, {} as Record<string, string>);
//       }, [contextSpecimens, specimens.entities]);
//     },
//   };
// 
//   return <DynamicField property={'barcode'} hook={specimens} field={field}/>
// }

export const SpecimenField: React.FC<{
  property: keyof ISpecimen,
  isStatic?: boolean
}> = ({ property, isStatic }) => {
  const specimen = useSpecimenContext();
  const field = getSpecimenFieldConfig(specimen)[property];
  return <DynamicField property={property} hook={specimen} field={field} isStatic={isStatic}/>
}

type ProcessFields = Pick<IProcess, 'protocol' | 'quantity' | 'unit' |
  'examiner' | 'date' | 'time' | 'comments'>;
type ProcessFieldConfig = FieldConfiguration<IProcess>;

const processFieldConfig: Record<keyof ProcessFields, ProcessFieldConfig> = {
  protocol: {
    label: 'Protocol',
    type: 'select',
    required: true,
    getOptions: (context) => {
      return {filler: 'filler'};
     // let specimenProtocols = {};
     // Object.entries(context.options.specimen?.protocols as { [key: string]: Protocol }).forEach(([id, protocol]) => {
     //   // FIXME: I really don't like 'toLowerCase()' function, but it's the
     //   // only way I can get it to work at the moment.
     //   if (typeId == protocol.typeId &&
     //       context.options.specimen.processes[protocol.processId].label.toLowerCase() ==
     //       processStage) {
     //     specimenProtocols[id] = protocol.label;
     //   }
     // });
     // return specimenProtocols;
    },
  },
  quantity: {
    label: 'Quantity',
    type: 'text',
    required: true,
  },
  unit: {
    label: 'Unit',
    type: 'select',
    required: true,
    getOptions: (context)  => {
    // XXX: THIS IS A HUGE PROBLEM BECAUSE THE SPECIMEN TYPE NEEDS TO BE KNOWN!
    // Object.keys(context.options.specimen.typeUnits[entity.typeId]||{})
    //  .reduce((result, id) => {
    //    result[id] = context.options.specimen.typeUnits[entity.typeId][id].label;
    //    return result;
    //  }, {}),
      return {filler: 'filler'};
    },
  },
  examiner: {
    label: 'Done By',
    type: 'text',
    required: true,
    getOptions: context => useRequest(new BaseAPI('examiner')),
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
  property: keyof IProcess,
  process: ProcessHook,
}> = ({
  property,
  process
}) => {
  const field = processFieldConfig[property];
  return <DynamicField property={property} hook={process} field={field}/>
}


const dataFieldConfig: Record<keyof IData, FieldConfiguration<IData>> = {
};

export const DataField: React.FC<{
  property: keyof IData,
  data: DataHook,
}> = ({
  property,
  data
}) => {
  const field = dataFieldConfig[property];
  return <DynamicField property={property} hook={data} field={field}/>
}
