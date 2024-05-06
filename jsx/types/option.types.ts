// TODO: ELIMINATE THIS EVENUTALLY BY MOVING THINGS TO WHERE THEY SHOULD BE 
import { Dimension } from '../entities';

export type Status = {
  id: string,
  label: string,
}

export type Protocol = {
   label: string,
   type: string,
   process: string,
 };

export type Candidate = {
   id: string,
   pscid: string,
   sex: string,
   diagnosis: string[],
};

export type User ={
  label: string,
}

export type Session = {
  id: string,
  label: string,
}

export type Options = {
  candidates: Record<string, Candidate>,
  diagnoses: Record<string, any>,
  sessions: Record<string, any>,
  projects: Record<string, any>,
  centers: Record<string, string>,
  examiners: Record<string, any>,
  users: Record<string, User>,
  candidateSessions: Record<string, any>,
  sessionCenters: Record<string, any>,
  container: {
    types: Record<string, any>,
    typesPrimary: Record<string, any>,
    typesNonPrimary: Record<string, any>,
    dimensions: Record<string, Dimension>,
    stati: Record<string, Status>,
  },
  specimen: {
    types: Record<string, any>,
    typeUnits: Record<string, any>,
    typeContainerTypes: Record<string, any>,
    protocols: Record<string, Protocol>,
    processes: Record<string, any>,
    protocolAttributes: Record<string, any>,
    processAttributes: Record<string, any>,
    units: Record<string, any>,
    attributes: Record<string, any>,
    attributeDatatypes: Record<string, any>,
  },
  shipment: {
    statuses: Record<string, any>,
    types: Record<string, any>,
  },
};
