import BaseAPI from './BaseAPI';
import { Specimen } from '../types';

class SpecimenAPI extends BaseAPI<Specimen> {
  constructor() {
    super('/biobank/specimens'); // Provide the base URL for specimen-related API
  }
}

export default SpecimenAPI;
