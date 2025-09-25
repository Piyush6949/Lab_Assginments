CREATE TABLE IF NOT EXISTS Library (
	id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	name TEXT,
	createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS Library_Audit (
	auditID BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,
	id BIGINT,
	name TEXT,
	createdAt TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);


DELIMITER //

CREATE TRIGGER lib_audit_before_update
BEFORE UPDATE ON Library
FOR EACH ROW
BEGIN
    INSERT INTO Library_Audit (id, name) VALUES (OLD.id, OLD.name);
END; //

DELIMITER ;


DELIMITER //

CREATE TRIGGER lib_audit_before_delete
BEFORE DELETE ON Library
FOR EACH ROW
BEGIN
    INSERT INTO Library_Audit (id, name) VALUES (OLD.id, OLD.name);
END; //

DELIMITER ;

-- Insert some sample data into Library
INSERT INTO Library (name) VALUES ('Algorithms Book');
INSERT INTO Library (name) VALUES ('Database Systems');

-- Check Library data
SELECT * FROM Library;

-- 🔹 Test UPDATE (this should fire the "before update" trigger)
UPDATE Library
SET name = 'Advanced Algorithms'
WHERE id = 1;

-- 🔹 Test DELETE (this should fire the "before delete" trigger)
DELETE FROM Library
WHERE id = 2;

-- Check Library after update & delete
SELECT * FROM Library;

-- 🔹 Now check the audit table
SELECT * FROM Library_Audit;
