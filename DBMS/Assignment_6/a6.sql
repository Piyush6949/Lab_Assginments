CREATE TABLE N_EmpId(
    empId INT PRIMARY KEY,
    name VARCHAR(255)
);

CREATE TABLE O_EmpId(
    empId INT PRIMARY KEY,
    name VARCHAR(255)
);

DELIMITER //

CREATE PROCEDURE new_old()
BEGIN 
    DECLARE name1 VARCHAR(255);
    DECLARE empId1 INT;
    DECLARE done INT DEFAULT false;

    DECLARE cur CURSOR FOR 
        SELECT empId, name 
        FROM N_EmpId n
        WHERE NOT EXISTS (
            SELECT 1 FROM O_EmpId o WHERE o.empId = n.empId
        );

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = true;

    OPEN cur;

    loop_label: LOOP
        FETCH cur INTO empId1, name1;
        IF done THEN
            LEAVE loop_label;
        END IF;

        INSERT INTO O_EmpId (empId, name)
        VALUES (empId1, name1);
    END LOOP;

    CLOSE cur;
END; //

DELIMITER ;

-- Insert data into N_EmpId
INSERT INTO N_EmpId (empId, name) VALUES
(101, 'Alice'),
(102, 'Bob'),
(103, 'Charlie'),
(104, 'David');

-- Insert data into O_EmpId
INSERT INTO O_EmpId (empId, name) VALUES
(103, 'Charlie'),
(104, 'David'),
(105, 'Eva'),
(106, 'Frank');

-- Run procedure
CALL new_old();

-- Check results
SELECT * FROM N_EmpId;
SELECT * FROM O_EmpId;
