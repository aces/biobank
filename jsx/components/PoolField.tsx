import React, { useState, useEffect } from 'react';
import { FieldConfiguration } from '../types';
import { Pool, IPool, PoolHook } from '../entities';
import { useRequest } from '../hooks';
import { BaseAPI } from '../APIs';
import { DynamicField } from '../components';
import { mapFormOptions } from '../utils';

type PoolFields = Pick<IPool, 'id' | 'label' | 'quantity' | 'unit' |
    'date' | 'time'>;

const poolFieldConfig: Record<keyof PoolFields, FieldConfiguration<IPool>> = {
  id: {
    label: 'Pool',
    type: 'search',
    required: true,
    getOptions: (context) => mapFormOptions(context.pools, 'label')
  },
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
    getOptions: (context) => useRequest(new BaseAPI('unit')),
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
}

export const PoolField: React.FC<{
  property: keyof IPool,
  pool: PoolHook,
}> = ({
  property,
  pool
}) => {
  const field = poolFieldConfig[property];
  return <DynamicField property={property} hook={pool} field={field}/>
}
