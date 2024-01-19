// TODO: ELIMINATE THIS EVENUTALLY BY MOVING THINGS TO WHERE THEY SHOULD BE 

export type Status = {
  id: string,
  label: string,
}

export type Dimension = {
  y: number,
  x: number,
  z: number,
  xNum: number,
  yNum: number,
  zNum: number,
};

type Protocol = {
   typeId: string,
   processId: string,
   // Include other properties that a protocol might have
 };

export type Options = {
  candidates: {[key: string]: any},
  diagnoses: {[key: string]: any},
  sessions: {[key: string]: any},
  projects: {[key: string]: any},
  centers: {[key: string]: any},
  examiners: {[key: string]: any},
  users: {[key: string]: any},
  candidateSessions: {[key: string]: any},
  sessionCenters: {[key: string]: any},
  container: {
   types: {[key: string]: any},
   typesPrimary: {[key: string]: any},
   typesNonPrimary: {[key: string]: any},
   dimensions: { [key: string]: Dimension },
   stati: { [key: string]: Status },
  },
  specimen: {
    types: {[key: string]: any},
    typeUnits: {[key: string]: any},
    typeContainerTypes: {[key: string]: any},
    protocols: {[key: string]: Protocol},
    processes: {[key: string]: any},
    protocolAttributes: {[key: string]: any},
    processAttributes: {[key: string]: any},
    units: {[key: string]: any},
    attributes: {[key: string]: any},
    attributeDatatypes: {[key: string]: any},
  },
  shipment: {
    statuses: {[key: string]: any},
    types: {[key: string]: any},
  },       
};
