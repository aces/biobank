import Utils from './';

export const normalizeOptions = <T>(
  options: T[],
  format: (option: T) => string 
): string[] => {
  return options.map(option => {
    if (typeof option === 'object' && option !== null) {
      return format(option);  // Apply format function to object options
    } else {
      return String(option);  // Apply format to string or primitive type
    }          
  })
};
