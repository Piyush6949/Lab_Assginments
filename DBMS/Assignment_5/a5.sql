CREATE TABLE Stud_Marks(
rollNo INT,
name VARCHAR(255),
totalMarks INT);

CREATE TABLE Result(
rollNo INT PRIMARY KEY,
name VARCHAR(255),
class VARCHAR(255));


create procedure proc_grade(IN roll_no INT, IN name1 VARCHAR(255), IN marks INT)
BEGIN
	INSERT INTO Stud_Marks (rollNo,name,totalMarks) 
	VALUES (roll_no, name1,marks);
	DECLARE class1 VARCHAR(255);
	DECLARE marks INT;
	DECLARE EXIT HANDLER FOR SQLEXCEPTION
    		BEGIN
        	SELECT 'Invalid marks' AS Message;
   		 END;
	
	SELECT totalMarks INTO marks 
	FROM Stud_Marks WHERE rollNo = roll_no;
	
        IF marks > 1500 THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'Invalid marks';
    	END IF;
    	
    	IF marks >= 990 THEN 
    	SET class1 = 'Distinction';
    	
    	ELSEIF marks >= 900 THEN
    	SET class1 = 'First class';
    	
    	ELSEIF marks >= 825 THEN
    	SET class1 = 'Higher Secondary Class';
    	
    	ELSE
    	SET class1 = 'Fail';
    	
    	END IF;
    	
    	INSERT INTO Result (rollNo, name, class)
    	VALUES (roll_no, name, class1);
    	SELECT 'Result Calculated Successfully' AS Message;
END;
;

TRUNCATE TABLE `Results`;

CALL proc_grade(1);
CALL proc_grade(2);
CALL proc_grade(3);
CALL proc_grade(4);
CALL proc_grade(5);
CALL proc_grade(6);
