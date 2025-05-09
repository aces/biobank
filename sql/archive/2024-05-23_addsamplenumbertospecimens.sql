-- Add a new column 'SampleNumber' to the 'biobank_specimen' table
ALTER TABLE biobank_specimen ADD COLUMN SampleNumber INT;

-- Update SampleNumber using a subquery based on mutual candidates
UPDATE biobank_specimen bs
JOIN (
    SELECT
        bs2.SpecimenID,
        (@row_number := IF(@cand_id = s.CandID, @row_number + 1, 1)) AS SampleNumber,
        (@cand_id := s.CandID) AS CandID
    FROM
        biobank_specimen bs2
    JOIN
        session s ON bs2.SessionID = s.ID
    CROSS JOIN (SELECT @row_number := 0, @cand_id := NULL) AS vars
    ORDER BY
        s.CandID, bs2.SpecimenID
) AS ns ON bs.SpecimenID = ns.SpecimenID
SET bs.SampleNumber = ns.SampleNumber;

-- Modify the SampleNumber column to be NOT NULL
ALTER TABLE biobank_specimen MODIFY COLUMN SampleNumber INT NOT NULL;
