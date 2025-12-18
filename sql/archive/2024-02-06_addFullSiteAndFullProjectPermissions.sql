INSERT INTO permissions (code, description, moduleID, action, categoryID)
VALUES ('biobank_fullsiteaccess', 'Full Site Access', (SELECT ID FROM modules WHERE Name = 'Biobank'), 'View', (SELECT ID FROM permissions_category WHERE Description = 'Permission'));

INSERT INTO permissions (code, description, moduleID, action, categoryID)
VALUES ('biobank_fullprojectaccess', 'Full Project Access', (SELECT ID FROM modules WHERE Name = 'Biobank'), 'View', (SELECT ID FROM permissions_category WHERE Description = 'Permission'));
