import BaseAPI from './BaseAPI';
import { Options } from '../types'; // Assuming you have a User type

export default class OptionAPI extends BaseAPI<Options> {
  constructor() {
    super('/options'); // Provide the base URL for container-related API
  }
}
