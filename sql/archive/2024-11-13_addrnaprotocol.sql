START TRANSACTION;

-- 1. Insert Specimen Type "RNA"
INSERT INTO biobank_specimen_type (Label, FreezeThaw)
VALUES ('RNA', 1);

-- 2. Establish Parent-Child Relationship for "RNA"
INSERT INTO biobank_specimen_type_parent (SpecimenTypeID, ParentSpecimenTypeID)
VALUES (
    (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'RNA'),
    (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'Blood')
);

-- 3. Insert New Attributes for RNA Collection and Preparation
INSERT INTO biobank_specimen_attribute (Label, DatatypeID)
VALUES 
    ('RNA Quantification Date', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'date')
    ),
    ('RNA Concentration (ng/µL)', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'number')
    ),
    ('Centrifuge Start #5', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #5', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #6', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #6', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #7', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #7', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #8', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #8', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #9', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #9', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #10', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #10', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #11', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #11', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge Start #12', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Centrifuge End #12', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Incubation Start #4', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Incubation End #4', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'time')
    ),
    ('Paxgene Blood miRNA Kit - Lot Number', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'text')
    ),
    ('Paxgene Blood miRNA Kit - Expiry Date', 
        (SELECT DatatypeID FROM biobank_specimen_attribute_datatype WHERE Datatype = 'date')
    );

-- 4. Insert RNA Collection and Preparation Protocols
INSERT INTO biobank_specimen_protocol (Label, SpecimenProcessID, SpecimenTypeID)
VALUES 
    ('CBIG-02-014 (RNA Extraction from Whole Blood)', 
        (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Collection'),
        (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'RNA')
    ),
    ('CBIG-02-014 (RNA Extraction from Whole Blood)', 
        (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation'),
        (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'RNA')
    );

-- 5. Link Attributes to RNA Collection Protocol
INSERT INTO biobank_specimen_protocol_attribute_rel (SpecimenProtocolID, SpecimenAttributeID, Required)
VALUES
    -- RNA Quantification Date
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Collection')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'RNA Quantification Date'),
        0
    ),
    -- RNA Concentration (ng/µL)
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Collection')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'RNA Concentration (ng/µL)'),
        0
    ),
    -- 260/280 Ratio
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Collection')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = '260/280 Ratio'),
        0
    );

-- 6. Link Attributes to RNA Preparation Protocol
INSERT INTO biobank_specimen_protocol_attribute_rel (SpecimenProtocolID, SpecimenAttributeID, Required)
VALUES
    -- Tube Expired
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Tube Expired'),
        0
    ),
    -- Incubation Start #1
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation Start #1'),
        1
    ),
    -- Incubation End #1
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation End #1'),
        1
    ),
    -- Centrifuge Start #1
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #1'),
        1
    ),
    -- Centrifuge End #1
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #1'),
        1
    ),
    -- Centrifuge Start #2
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #2'),
        1
    ),
    -- Centrifuge End #2
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #2'),
        1
    ),
    -- Incubation Start #2
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation Start #2'),
        1
    ),
    -- Incubation End #2
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation End #2'),
        1
    ),
    -- Centrifuge Start #3
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #3'),
        1
    ),
    -- Centrifuge End #3
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #3'),
        1
    ),
    -- Centrifuge Start #4
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #4'),
        1
    ),
    -- Centrifuge End #4
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #4'),
        1
    ),
    -- Centrifuge Start #5
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #5'),
        1
    ),
    -- Centrifuge End #5
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #5'),
        1
    ),
    -- Centrifuge Start #6
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #6'),
        1
    ),
    -- Centrifuge End #6
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #6'),
        1
    ),
    -- Incubation Start #3
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation Start #3'),
        1
    ),
    -- Incubation End #3
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation End #3'),
        1
    ),
    -- Centrifuge Start #7
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #7'),
        1
    ),
    -- Centrifuge End #7
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #7'),
        1
    ),
    -- Centrifuge Start #8
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #8'),
        1
    ),
    -- Centrifuge End #8
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #8'),
        1
    ),
    -- Centrifuge Start #9
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #9'),
        1
    ),
    -- Centrifuge End #9
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #9'),
        1
    ),
    -- Centrifuge Start #10
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #10'),
        1
    ),
    -- Centrifuge End #10
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #10'),
        1
    ),
    -- Centrifuge Start #11
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #11'),
        1
    ),
    -- Centrifuge End #11
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #11'),
        1
    ),
    -- Centrifuge Start #12
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge Start #12'),
        1
    ),
    -- Centrifuge End #12
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Centrifuge End #12'),
        1
    ),
    -- Incubation Start #4
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation Start #4'),
        1
    ),
    -- Incubation End #4
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Incubation End #4'),
        1
    ),
    -- Paxgene Blood miRNA Kit - Lot Number
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Paxgene Blood miRNA Kit - Lot Number'),
        0
    ),
    -- Paxgene Blood miRNA Kit - Expiry Date
    (
        (SELECT SpecimenProtocolID FROM biobank_specimen_protocol 
         WHERE Label = 'CBIG-02-014 (RNA Extraction from Whole Blood)' 
           AND SpecimenProcessID = (SELECT SpecimenProcessID FROM biobank_specimen_process WHERE Label = 'Preparation')),
        (SELECT SpecimenAttributeID FROM biobank_specimen_attribute WHERE Label = 'Paxgene Blood miRNA Kit - Expiry Date'),
        0
    );

-- 7. Associate Units with the "RNA" Specimen Type
UPDATE biobank_unit
SET Label = 'µL'
WHERE UnitID = 1;

-- Link 'RNA' to 'µL'
INSERT INTO biobank_specimen_type_unit_rel (SpecimenTypeID, UnitID)
VALUES (
    (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'RNA'),
    (SELECT UnitID FROM biobank_unit WHERE Label = 'µL')
);

-- 8. Associate Container Types with the "RNA" Specimen Type
-- Link 'RNA' to 'Cryotube Vial'
INSERT INTO biobank_specimen_type_container_type_rel (SpecimenTypeID, ContainerTypeID)
VALUES (
    (SELECT SpecimenTypeID FROM biobank_specimen_type WHERE Label = 'RNA'),
    (SELECT ContainerTypeID FROM biobank_container_type WHERE Label = 'Cryotube Vial')
);

COMMIT;
