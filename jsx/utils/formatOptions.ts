import Utils from './';

export const formatOptions = <T>(
  options: Record<string, T> = {},
  format: (option: T) => string = (option) => String(option)
): Record<string, string> => {
  return Object.entries(options).reduce((acc, [key, option]) => {          
    acc[key] = format(option); 
    return acc;
  }, {}); // Start with an empty object
};
