import React, { useState, useEffect } from 'react';
import { FieldConfiguration } from '../types';
import { ISpecimen, SpecimenHook, useSpecimenContext, useProcessContext, IProcess, ProcessHook, IData, DataHook } from '../entities';
import { DynamicField } from '../forms';
import { useRequest } from '../hooks';
import Utils from '../utils';
import { SpecimenAPI, BaseAPI } from '../APIs';
import { Operator } from '../APIs/Query';
import { mapFormOptions } from '../utils';

type SpecimenFields = Pick<ISpecimen, 'candidate' | 'session' | 'type' | 'quantity' | 'unit' | 'fTCycle' |
  'projects'>;

const getSpecimenFieldConfig = (
  specimen: SpecimenHook
): Record<keyof SpecimenFields, FieldConfiguration<ISpecimen, keyof SpecimenFields>> => ({
  candidate: {
    label: "PSCID",
    type: 'search',
    required: true,
    placeholder: 'Search for a PSCID',
    getOptions: () => Utils.mapLabel(useRequest(new BaseAPI('candidates').get())),
  },
  session: {
    label: 'Visit Label',
    type: 'select',
    required: true,
    disabled: !specimen.candidate,
    // autoSelect: true,
    getOptions: (context) => {
      return specimen.candidate ?
        Utils.mapLabel(useRequest(new BaseAPI('sessions').get({field: 'candidate', value:
                                    specimen.candidate.label})))
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
        return Utils.mapLabel(useRequest(new SpecimenAPI().getTypes({
          field: 'parents',
          value: specimen.parents[0].type.label,
          operator: Operator.Includes 
        })));
      }

      return Utils.mapLabel(useRequest(new SpecimenAPI().getTypes()));
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
      return specimen.type ? Utils.mapLabel(specimen.type.units) : {};
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
    getOptions: () => useRequest(new BaseAPI('projects').get()),
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
type ProcessFieldConfig = FieldConfiguration<IProcess, keyof ProcessFields>;

const getProcessFieldConfig = (
  specimen: SpecimenHook
): Record<keyof ProcessFields, ProcessFieldConfig> => ({
  protocol: {
    label: 'Protocol',
    type: 'select',
    required: true,
    getOptions: (context)  => {
       return Utils.mapLabel(useRequest(new SpecimenAPI().getProtocols())
       .find(protocol => protocol.type.label === specimen.type.label));
       // TODO: I need to add to this that it matches the current process stage.
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
       return Utils.mapLabel(useRequest(new SpecimenAPI().getTypes())
       .find(type => type.label === specimen.type.label));
    },
  },
  examiner: {
    label: 'Done By',
    type: 'text',
    required: true,
    getOptions: () => Utils.mapLabel(useRequest(new BaseAPI('examiner').get())),
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
});

export const ProcessField: React.FC<{
  property: keyof IProcess,
}> = ({
  property,
}) => {
  const specimen = useSpecimenContext()
  const process = useProcessContext();
  const field = getProcessFieldConfig(specimen)[property];
  return <DynamicField property={property} hook={process} field={field}/>;
}


const dataFieldConfig: Record<keyof IData, FieldConfiguration<IData, keyof IData>> = {
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
