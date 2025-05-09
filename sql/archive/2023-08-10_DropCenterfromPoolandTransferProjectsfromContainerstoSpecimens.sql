
-- DROP Center From Pools
ALTER TABLE biobank_pool
DROP FOREIGN KEY FK_biobank_pool_CenterID,
DROP COLUMN CenterID;

-- Add new specimen project relational table
CREATE TABLE `biobank_specimen_project_rel` (
    `SpecimenID` integer unsigned NOT NULL,
    `ProjectID` int(10) unsigned NOT NULL,
    CONSTRAINT `PK_biobank_specimen_project_rel` PRIMARY KEY (`SpecimenID`, `ProjectID`),
    CONSTRAINT `FK_biobank_specimen_project_rel_SpecimenID`
        FOREIGN KEY (`SpecimenID`) REFERENCES `biobank_specimen`(`SpecimenID`)
        ON UPDATE RESTRICT ON DELETE RESTRICT,
    CONSTRAINT `FK_biobank_specimen_project_rel_ProjectID`
        FOREIGN KEY (`ProjectID`) REFERENCES `Project`(`ProjectID`)
        ON UPDATE RESTRICT ON DELETE RESTRICT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Insert projects and specimens
INSERT INTO biobank_specimen_project_rel (SpecimenID, ProjectID)
SELECT bsp.SpecimenID, rcp.ProjectID
FROM biobank_container_project_rel rcp
LEFT JOIN biobank_container bc ON rcp.ContainerID = bc.ContainerID
RIGHT JOIN biobank_specimen bsp ON bc.ContainerID = bsp.ContainerID;

-- Drop old table
DROP TABLE biobank_container_project_rel;
