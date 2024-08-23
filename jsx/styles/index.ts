// re-export exports from each file
export * from './button';
export * from './form';
export * from './layout';

// Import default export from each file
import Button from './button';
import Form from './form';
import Layout from './layout';

// Create a combind default export object
const Styles = {
    Button,
    Form,
    Layout
};

// Export the object as the default export
export default Styles;
