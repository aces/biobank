import BaseAPI from './BaseAPI';
import { Options } from '../types'; // Assuming you have a User type

class OptionAPI extends BaseAPI<Options> {
  constructor() {
    super('/biobank/optionss'); // Provide the base URL for container-related API
  }
}

export default OptionAPI;
