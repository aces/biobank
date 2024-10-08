// Re-export exports from each file
export * from './clone';
export * from './mapFormOptions';
export * from './formatOptions';
export * from './isEmpty';
export * from './padBarcode';

// Import default export from each file
import { clone } from './clone';
import { mapFormOptions } from './mapFormOptions';
import { mapLabel } from './mapFormOptions';
import { formatOptions } from './formatOptions';
import { isEmpty } from './isEmpty';
import { padBarcode } from './padBarcode';

// Create a combind default export object
const Utils = {
    clone,
    mapLabel,
    mapFormOptions,
    formatOptions,
    isEmpty,
    padBarcode
};

// Export the object as the default export
export default Utils;
