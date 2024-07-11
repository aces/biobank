import BaseAPI from './BaseAPI';
import { Label } from '../types'; // Assuming you have a User type

export default class LabelAPI extends BaseAPI<Label> {
  constructor() {
    super('/labels'); // Provide the base URL for label-related API
  }
}
