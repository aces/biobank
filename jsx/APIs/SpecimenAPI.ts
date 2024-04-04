import BaseAPI from './BaseAPI';
import { Specimen } from '../types';

export default class SpecimenAPI extends BaseAPI<Specimen> {
  constructor() {
    super('/biobank/specimens');
  }
}
