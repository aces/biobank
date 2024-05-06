import BaseAPI from './BaseAPI';
import { Specimen } from '../entities';

export default class SpecimenAPI extends BaseAPI<Specimen> {
  constructor() {
    super('/biobank/specimens');
  }
}
