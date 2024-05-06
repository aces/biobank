import { useState, useEffect, useRef } from 'react';
import { IAPI } from '../APIs';
import { useRequest } from './';

export const useStream = <T>(api: IAPI<T>) => {
  const [progress, setProgress] = useState(0);
  const abortController = useRef(new AbortController());
  const key = api.constructor.name;

  const { data, error } = useRequest([key], () => {
    return api.fetchStream(setProgress, abortController.current.signal);
  });

  useEffect(() => {
    return () => {
      abortController.current.abort();  // Abort fetch on component unmount
    };
  }, []);

  return { data, progress, error };
};
