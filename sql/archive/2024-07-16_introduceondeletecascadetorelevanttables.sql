-- Update foreign keys referencing biobank_container
ALTER TABLE biobank_specimen
DROP FOREIGN KEY FK_biobank_specimen_ContainerID;

ALTER TABLE biobank_specimen
ADD CONSTRAINT FK_biobank_specimen_ContainerID
FOREIGN KEY (ContainerID)
REFERENCES biobank_container(ContainerID)
ON DELETE CASCADE;

ALTER TABLE biobank_container_shipment_rel
DROP FOREIGN KEY FK_shipment_log_ContainerID;

ALTER TABLE biobank_container_shipment_rel
ADD CONSTRAINT FK_shipment_log_ContainerID
FOREIGN KEY (ContainerID)
REFERENCES biobank_container(ContainerID)
ON DELETE CASCADE;

ALTER TABLE biobank_container_parent
DROP FOREIGN KEY FK_biobank_container_parent_ContainerID;

ALTER TABLE biobank_container_parent
ADD CONSTRAINT FK_biobank_container_parent_ContainerID
FOREIGN KEY (ContainerID)
REFERENCES biobank_container(ContainerID)
ON DELETE CASCADE;

ALTER TABLE biobank_container_parent
DROP FOREIGN KEY FK_biobank_container_parent_ParentContainerID;

ALTER TABLE biobank_container_parent
ADD CONSTRAINT FK_biobank_container_parent_ParentContainerID
FOREIGN KEY (ParentContainerID)
REFERENCES biobank_container(ContainerID)
ON DELETE CASCADE;

-- Update foreign keys referencing biobank_pool
ALTER TABLE biobank_specimen_pool_rel
DROP FOREIGN KEY FK_biobank_specimen_pool_rel_PoolID;

ALTER TABLE biobank_specimen_pool_rel
ADD CONSTRAINT FK_biobank_specimen_pool_rel_PoolID
FOREIGN KEY (PoolID)
REFERENCES biobank_pool(PoolID)
ON DELETE CASCADE;

-- Update foreign keys referencing biobank_specimen
ALTER TABLE biobank_specimen_project_rel
DROP FOREIGN KEY FK_biobank_specimen_project_rel_SpecimenID;

ALTER TABLE biobank_specimen_project_rel
ADD CONSTRAINT FK_biobank_specimen_project_rel_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_freezethaw
DROP FOREIGN KEY FK_biobank_specimen_freezethaw_SpecimenID;

ALTER TABLE biobank_specimen_freezethaw
ADD CONSTRAINT FK_biobank_specimen_freezethaw_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_parent
DROP FOREIGN KEY FK_biobank_specimen_parent_ParentSpecimenID;

ALTER TABLE biobank_specimen_parent
ADD CONSTRAINT FK_biobank_specimen_parent_ParentSpecimenID
FOREIGN KEY (ParentSpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_parent
DROP FOREIGN KEY FK_biobank_specimen_parent_SpecimenID;

ALTER TABLE biobank_specimen_parent
ADD CONSTRAINT FK_biobank_specimen_parent_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_collection
DROP FOREIGN KEY FK_biobank_specimen_collection_SpecimenID;

ALTER TABLE biobank_specimen_collection
ADD CONSTRAINT FK_biobank_specimen_collection_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_preparation
DROP FOREIGN KEY FK_biobank_specimen_preparation_SpecimenID;

ALTER TABLE biobank_specimen_preparation
ADD CONSTRAINT FK_biobank_specimen_preparation_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_analysis
DROP FOREIGN KEY FK_biobank_specimen_analysis_SpecimenID;

ALTER TABLE biobank_specimen_analysis
ADD CONSTRAINT FK_biobank_specimen_analysis_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

ALTER TABLE biobank_specimen_pool_rel
DROP FOREIGN KEY FK_biobank_specimen_pool_rel_SpecimenID;

ALTER TABLE biobank_specimen_pool_rel
ADD CONSTRAINT FK_biobank_specimen_pool_rel_SpecimenID
FOREIGN KEY (SpecimenID)
REFERENCES biobank_specimen(SpecimenID)
ON DELETE CASCADE;

