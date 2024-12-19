ALTER TABLE biobank_specimen_protocol_attribute_rel
ADD COLUMN OrderIndex INT UNSIGNED NULL;

-- Protocol: BB-P-0001 (PBMC isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 19; -- DNA Quantification Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 21; -- 260/280 Ratio

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0002 (DNA extraction from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 16 AND SpecimenAttributeID = 19; -- DNA Quantification Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 16 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 16 AND SpecimenAttributeID = 21; -- 260/280 Ratio

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 16 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0009 (DNA extraction from Saliva)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 17 AND SpecimenAttributeID = 13; -- Red Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 17 AND SpecimenAttributeID = 18; -- No Visible Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 17 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 17 AND SpecimenAttributeID = 21; -- 260/280 Ratio

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 17 AND SpecimenAttributeID = 63; -- Saliva Date Received

-- Protocol: CBIG-02-004 (DNA Extraction from Whole Blood (Accelerated Method))
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 64 AND SpecimenAttributeID = 19; -- DNA Quantification Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 64 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 64 AND SpecimenAttributeID = 21; -- 260/280 Ratio

-- Protocol: CBIG-02-006 (DNA Extraction from Saliva)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 67 AND SpecimenAttributeID = 13; -- Red Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 67 AND SpecimenAttributeID = 18; -- No Visible Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 67 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 67 AND SpecimenAttributeID = 21; -- 260/280 Ratio

-- Protocol: ETP-P-0003 (DNA extraction from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 46 AND SpecimenAttributeID = 20; -- DNA Concentration (ng/µL)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 46 AND SpecimenAttributeID = 21; -- 260/280 Ratio

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 46 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0006 (Fibroblast culture)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 23 AND SpecimenAttributeID = 60; -- Skin Biopsy Date Received

-- Protocol: CBIG-02-013 (Fibroblast Culture)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 29; -- Live Cells(%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 30; -- Dead Cells(%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 38; -- Cell Count (million cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 98; -- PBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 101; -- PBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 143; -- Fibroblast Medium: DMEM - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 146; -- Fibroblast Medium: DMEM - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 149; -- Fibroblast Medium: FBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 152; -- Fibroblast Medium: FBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 155; -- Fibroblast Medium: Pen/Strep - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 158; -- Fibroblast Medium: Pen/Strep - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 161; -- Fibroblast Medium: Amphotericine - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 164; -- Fibroblast Medium: Amphotericine - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 167; -- Freezing Medium: FBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 170; -- Freezing Medium: FBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 173; -- Freezing Medium: DMSO - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 176; -- TrypleExpress - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 104 AND SpecimenAttributeID = 179; -- TrypleExpress - Expiry Date

-- Protocol: EDDU
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 53 AND SpecimenAttributeID = 35; -- Cell Line Name

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 53 AND SpecimenAttributeID = 38; -- Cell Count (million cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 53 AND SpecimenAttributeID = 41; -- Viability (%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 53 AND SpecimenAttributeID = 44; -- Passage Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 53 AND SpecimenAttributeID = 47; -- Reprogramming

-- Protocol: BB-P-0001 (PBMC Isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 14; -- Total PBMC Count (10⁶/mL cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 18 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0014 (PBMC isolation from Whole Blood (Leucosep method))
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 19 AND SpecimenAttributeID = 14; -- Total PBMC Count (10⁶/mL cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 19 AND SpecimenAttributeID = 29; -- Live Cells(%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 19 AND SpecimenAttributeID = 30; -- Dead Cells(%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 19 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: CBIG-02-001 (PBMC Isolation from Whole Blood - Conventional Method)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 70 AND SpecimenAttributeID = 14; -- Total PBMC Count (10⁶/mL cells)

-- Protocol: CBIG-02-002 (PBMC Isolation from Whole Blood - Leucosep Method)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 73 AND SpecimenAttributeID = 14; -- Total PBMC Count (10⁶/mL cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 73 AND SpecimenAttributeID = 29; -- Live Cells(%)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 73 AND SpecimenAttributeID = 30; -- Dead Cells(%)

-- Protocol: ETP-P-0001 (PBMC isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 36 AND SpecimenAttributeID = 14; -- Total PBMC Count (10⁶/mL cells)

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 36 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0004 (Plasma isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 58 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 58 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 58 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 58 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 58 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: ETP-P-0011 (Plasma isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 40 AND SpecimenAttributeID = 54; -- BHA

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 40 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: CBIG-02-014 (RNA Extraction from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 120 AND SpecimenAttributeID = 21; -- 260/280 Ratio

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 120 AND SpecimenAttributeID = 180; -- RNA Quantification Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 120 AND SpecimenAttributeID = 181; -- RNA Concentration (ng/µL)

-- Protocol: BB-P-0003 (Serum isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 21 AND SpecimenAttributeID = 15; -- Milky Serum

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 21 AND SpecimenAttributeID = 16; -- Hemolyzed

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 21 AND SpecimenAttributeID = 17; -- Hemodialysis Index

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 21 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: CBIG-02-003 (Serum Isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 76 AND SpecimenAttributeID = 15; -- Milky Serum

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 76 AND SpecimenAttributeID = 16; -- Hemolyzed

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 76 AND SpecimenAttributeID = 17; -- Hemodialysis Index

-- Protocol: ETP-P-0002 (Serum isolation from Whole Blood)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 38 AND SpecimenAttributeID = 15; -- Milky Serum

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 38 AND SpecimenAttributeID = 17; -- Hemodialysis Index

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 38 AND SpecimenAttributeID = 57; -- Blood Date Received

-- Protocol: BB-P-0007 (Skin Biopsy)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 24 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 24 AND SpecimenAttributeID = 4; -- Incubation End #1

-- Protocol: CBIG-02-012 Skin Biopsy (Sampling Procedure, Transport and Culture)
-- Process Stage: Collection

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 98 AND SpecimenAttributeID = 50; -- Transport Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 98 AND SpecimenAttributeID = 53; -- Transport End #1

-- Protocol: BB-P-0001 (PBMC Isolation from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 1; -- Clotted

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 25 AND SpecimenAttributeID = 13; -- Red Pellet

-- Protocol: BB-P-0002 (DNA extraction from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 11; -- Centrifuge Start #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 12; -- Centrifuge End #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 13; -- Red Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 18; -- No Visible Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 26; -- Airdry Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 27 AND SpecimenAttributeID = 27; -- Airdry End #1

-- Protocol: BB-P-0003 (Serum isolation from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 26 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 26 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 26 AND SpecimenAttributeID = 6; -- Centrifuge End #1

-- Protocol: CBIG-02-001 (PBMC Isolation from Whole Blood - Conventional Method)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 1; -- Clotted

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 82 AND SpecimenAttributeID = 13; -- Red Pellet

-- Protocol: CBIG-02-002 (PBMC Isolation from Whole Blood - Leucosep Method)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 1; -- Clotted

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 13; -- Red Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 92; -- Leucosep - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 95; -- Leucosep - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 98; -- PBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 101; -- PBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 104; -- Ficoll - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 107; -- Ficoll - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 110; -- HI HuAB Serum - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 113; -- HI HuAB Serum - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 79 AND SpecimenAttributeID = 116; -- DMSO - Lot Number

-- Protocol: CBIG-02-003 (Serum Isolation from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 85 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 85 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 85 AND SpecimenAttributeID = 6; -- Centrifuge End #1

-- Protocol: CBIG-02-004 (DNA Extraction from Whole Blood (Accelerated Method))
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 11; -- Centrifuge Start #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 12; -- Centrifuge End #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 13; -- Red Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 18; -- No Visible Pellet

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 26; -- Airdry Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 27; -- Airdry End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 65; -- RBC Lysis Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 68; -- RBC Lysis Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 71; -- Protein Precipitation Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 190
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 74; -- Protein Precipitation Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 200
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 77; -- Cell Lysis Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 210
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 80; -- Cell Lysis Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 220
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 83; -- DNA Hydration Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 230
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 86; -- DNA Hydration Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 240
WHERE SpecimenProtocolID = 88 AND SpecimenAttributeID = 89; -- 2-propanol - Lot Number

-- Protocol: CBIG-02-011 (Plasma Isolation from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 95 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 95 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 95 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 95 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 95 AND SpecimenAttributeID = 6; -- Centrifuge End #1

-- Protocol: CBIG-02-014 (RNA Extraction from Whole Blood)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 22; -- Incubation Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 23; -- Incubation End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 11; -- Centrifuge Start #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 12; -- Centrifuge End #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 182; -- Centrifuge Start #5

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 183; -- Centrifuge End #5

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 184; -- Centrifuge Start #6

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 185; -- Centrifuge End #6

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 24; -- Incubation Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 190
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 25; -- Incubation End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 200
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 186; -- Centrifuge Start #7

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 210
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 187; -- Centrifuge End #7

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 220
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 188; -- Centrifuge Start #8

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 230
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 189; -- Centrifuge End #8

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 240
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 190; -- Centrifuge Start #9

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 250
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 191; -- Centrifuge End #9

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 260
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 192; -- Centrifuge Start #10

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 270
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 193; -- Centrifuge End #10

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 280
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 194; -- Centrifuge Start #11

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 290
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 195; -- Centrifuge End #11

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 300
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 196; -- Centrifuge Start #12

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 310
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 197; -- Centrifuge End #12

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 320
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 198; -- Incubation Start #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 330
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 199; -- Incubation End #4

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 340
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 200; -- Paxgene Blood miRNA Kit - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 350
WHERE SpecimenProtocolID = 121 AND SpecimenAttributeID = 201; -- Paxgene Blood miRNA Kit - Expiry Date

-- Protocol: BB-P-0010 (CSF processing)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 28 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 28 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 28 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 28 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 28 AND SpecimenAttributeID = 28; -- Blood Contamination

-- Protocol: CBIG-02-008 (CSF Processing)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 91 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 91 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 91 AND SpecimenAttributeID = 28; -- Blood Contamination

-- Protocol: BB-P-0009 (DNA extraction from Saliva)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 22; -- Incubation Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 23; -- Incubation End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 24; -- Incubation Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 25; -- Incubation End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 26; -- Airdry Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 14 AND SpecimenAttributeID = 27; -- Airdry End #1

-- Protocol: CBIG-02-006 (DNA Extraction from Saliva)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 2; -- Tube Expired

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 4; -- Incubation End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 9; -- Centrifuge Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 10; -- Centrifuge End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 22; -- Incubation Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 23; -- Incubation End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 24; -- Incubation Start #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 25; -- Incubation End #3

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 26; -- Airdry Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 27; -- Airdry End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 71; -- Protein Precipitation Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 74; -- Protein Precipitation Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 77; -- Cell Lysis Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 190
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 80; -- Cell Lysis Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 200
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 83; -- DNA Hydration Solution - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 210
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 86; -- DNA Hydration Solution - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 220
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 89; -- 2-propanol - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 230
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 119; -- RNAse Solution A - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 240
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 122; -- RNAse Solution A - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 250
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 125; -- Oragene DNA Tube - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 260
WHERE SpecimenProtocolID = 61 AND SpecimenAttributeID = 128; -- Oragene DNA Tube - Expiry Date

-- Protocol: CBIG-02-012 Skin Biopsy (Sampling Procedure, Transport and Culture)
-- Process Stage: Preparation

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 10
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 3; -- Incubation Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 20
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 5; -- Centrifuge Start #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 30
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 6; -- Centrifuge End #1

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 40
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 7; -- Centrifuge Start #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 50
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 8; -- Centrifuge End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 60
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 23; -- Incubation End #2

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 70
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 131; -- Wash Solution: PBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 80
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 134; -- Wash Solution: PBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 90
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 137; -- Wash Solution: Pen/Strep - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 100
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 140; -- Wash Solution: Pen/Strep - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 110
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 143; -- Fibroblast Medium: DMEM - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 120
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 146; -- Fibroblast Medium: DMEM - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 130
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 149; -- Fibroblast Medium: FBS - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 140
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 152; -- Fibroblast Medium: FBS - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 150
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 155; -- Fibroblast Medium: Pen/Strep - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 160
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 158; -- Fibroblast Medium: Pen/Strep - Expiry Date

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 170
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 161; -- Fibroblast Medium: Amphotericine - Lot Number

UPDATE biobank_specimen_protocol_attribute_rel
SET OrderIndex = 180
WHERE SpecimenProtocolID = 101 AND SpecimenAttributeID = 164; -- Fibroblast Medium: Amphotericine - Expiry Date

-- Apply NOT NULL constraint
ALTER TABLE biobank_specimen_protocol_attribute_rel
MODIFY COLUMN OrderIndex INT UNSIGNED NOT NULL;

-- Add the unique constraint
ALTER TABLE biobank_specimen_protocol_attribute_rel
ADD CONSTRAINT UK_SpecimenProtocolId_OrderIndex
UNIQUE (SpecimenProtocolID, OrderIndex);
