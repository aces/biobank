import React, { useState, useEffect } from 'react';
import { FieldConfiguration } from '../types';
import { Pool, IPool, PoolHook, usePoolContext } from '../entities';
import { useRequest } from '../hooks';
import { BiobankContextType } from '../contexts';
import { BaseAPI } from '../APIs';
import { DynamicField } from '../forms';
import { mapFormOptions } from '../utils';

type PoolFields = Pick<IPool, 'label' | 'quantity' | 'unit' |
    'type' | 'candidate' | 'session' | 'specimens' | 'date' | 'time'>;
type PoolConfig = FieldConfiguration<IPool, keyof PoolFields>;

const poolFieldConfig: Record<keyof PoolFields, PoolConfig> = {
  label: {
    label: 'Label',
    type: 'text',
    required: true,
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
    getOptions: () => useRequest(new BaseAPI('unit').get()),
  },
  type: {
    label: 'Type',
    type: 'static',
  },
  candidate: {
    label: 'Candidate',
    type: 'static',
  },
  session: {
    label: 'Session',
    type: 'static',
  },
  specimens: {
    label: 'Specimens',
    type: 'search',
    required: true,
    getOptions: (context) => context.specimens.data,
    format: (specimen) => specimen.container.barcode
  } as FieldConfiguration<IPool, 'specimens'>, // this is so frustrating and
  // leads me to believe that each field might have to be split into its own type
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
}

export const PoolField: React.FC<{
  property: keyof IPool,
  isStatic?: boolean,
}> = ({
  property,
  isStatic,
}) => {
  const pool = usePoolContext();
  const field = poolFieldConfig[property];
  return <DynamicField property={property} hook={pool} field={field}
  isStatic={isStatic}/>
}
