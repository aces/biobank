import { useState, useEffect } from 'react';
import { IAPI } from '../APIs';

export const useStreamData = <T>(api: IAPI<T>) => {
  const [data, setData] = useState(null);
  const [progress, setProgress] = useState(0);
  const [error, setError] = useState(null);

  useEffect(() => {
    let isCancelled = false;

    const fetchData = async () => {
      try {
        const result = await api.streamData((prog) => {
          if (!isCancelled) {
            setProgress(prog);
          }
        });
        if (!isCancelled) {
          setData(result);
        }
      } catch (err) {
        if (!isCancelled) {
          setError(err);
        }
      }
    };

    fetchData();

    return () => {
      isCancelled = true;
    };
  }, [api]);

  return { data, progress, error };
};
