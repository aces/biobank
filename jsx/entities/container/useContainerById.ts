import { useState, useEffect } from 'react';
import { ContainerAPI } from '../APIs'; // Assume this is your API module
import { useContainer } from '../hooks'; // Your existing useContainer hook

// A custom hook that fetches container data by ID and initializes it with useContainer
export function useContainerById(barcode) {
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState(null);
  const [data, setData] = useState(null);

  useEffect(() => {
    async function fetchAndInitializeContainer() {
      try {
        setLoading(true);
        const fetchedData = await new ContainerAPI().getById(barcode);
        // Assuming fetchedData is directly usable as container state or
        // perform any transformation if needed
        setData(fetchedData);
      } catch (err) {
        setError(err);
      } finally {
        setLoading(false);
      }
    }

    if (barcode) {
      fetchAndInitializeContainer();
    }
  }, [barcode]);

  // Use the fetched data to initialize the container state with your useContainer hook
  // This step assumes useContainer can handle `null` or uninitialized data gracefully
  // until the actual container data is fetched
  const [container, containerHandler] = useContainer(new Container(data));

  return { container, containerHandler, loading, error };
}
