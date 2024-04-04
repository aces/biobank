import BaseAPI from './BaseAPI';
import { Label } from '../types'; // Assuming you have a User type

export default class LabelAPI extends BaseAPI<Label> {
  constructor() {
    super('/biobank/labels'); // Provide the base URL for label-related API
  }
}
