
--Demonstrate the use of concepts like all types of Join ,Sub-Query and View on the
--following Schema
--● Employee( Emp_id, Dept_id, Emp_fname, Emp_lname, Emp_Position,
--Emp_salary,Emp_JoinDate)
--● Dept ( Dept_id, Dept_name,location)
--● Project( Proj_id,Dept_id ,Proj_Name,Proj_Location,Proj_cost,Proj_year)

CREATE TABLE Department (
  deptId INT PRIMARY KEY,
  deptName VARCHAR(100) NOT NULL,
  deptLocation VARCHAR(100)
);

CREATE TABLE Employee3 (
    empId INT ,
    deptId INT,
    empFname VARCHAR(100) , 
    empLname VARCHAR(100) ,
    empPosition VARCHAR(100),       
    empSalary INT,
    empJoinDate DATE
);

CREATE TABLE Project(
  projId INT PRIMARY KEY,
  deptId INT,
  projName VARCHAR(100) NOT NULL,
  projLocation VARCHAR(100),
  projCost INT NOT NULL DEFAULT 0,
  projYear DATE NOT NULL
);


ALTER TABLE Employee2
    ALTER COLUMN empFname SET DEFAULT 'abc';

--1. Find Employee details and Department details using NATURAL JOIN.

SELECT * FROM Employee NATURAL JOIN Department;

--2. Find the emp_fname,Emp_position,location,Emp_JoinDate who have same Dept id.

SELECT empFname,empPosition,deptLocation,empJoinDate FROM Employee INNER JOIN Department ON Employee.deptId = Department.deptId;
-- add department id

--3. Find the Employee details ,Proj_id,Project cost who does not have Project location as ‘Hyderabad’.

SELECT Employee.*,projId,projCost FROM Employee NATURAL JOIN Project WHERE projLocation NOT LIKE 'Hyderabad';

--4. Find Department Name ,employee name, Emp_position for which project year is 2020.

CREATE VIEW problem4_1 AS SELECT * FROM Department NATURAL JOIN Project WHERE YEAR(projYear) = 2007;
SELECT deptName,empFname,empPosition FROM problem4 NATURAL JOIN Employee;

--5. Display emp_position,D_name who have Project cost >30000.

CREATE VIEW problem5 AS SELECT * FROM Department NATURAL JOIN Project WHERE projCost > 300000;
SELECT empPosition,deptName FROM problem5 NATURAL JOIN Employee;

--6. Find the names of all the Projects that started in the year 2004.

SELECT projName FROM Projects WHERE YEAR(projYear) = 2004;

--7. List the Dept_name having no_of_emp=3.

SELECT deptName FROM Employee NATURAL JOIN Department GROUP BY Employee.deptId HAVING COUNT(Employee.deptId) = 3;

--8. Display the total number of employee who have joined any project before 2007

SELECT COUNT(DISTINCT(empId)) FROM Employee NATURAL JOIN Project WHERE YEAR(projYear) < 2007;

--9. Create a view showing the employee and Department details.

CREATE VIEW problem9 AS SELECT * FROM Employee NATURAL JOIN Department;
SELECT * FROM problem9;

--10. Perform Manipulation on simple view-Insert, update, delete, drop view.
CREATE VIEW problem10 AS SELECT empId,empFname FROM Employee1;
UPDATE problem10 SET empFname = "Himani" WHERE empId = 8;
SELECT * FROM problem10;
INSERT INTO problem10 (empFname) VALUES ('Ved');
DELETE FROM problem10 WHERE empId = 4;
DROP VIEW problem10;

-- Using left Join
CREATE VIEW leftJoin AS
SELECT e.empFname, d.deptName
FROM Employee e
LEFT JOIN Department d ON e.deptId = d.deptId;

-- Using right Join
CREATE VIEW rightJoin AS
SELECT e.empFname, d.deptName
FROM Employee e
RIGHT JOIN Department d ON e.deptId = d.deptId;

-- Using full outer join
CREATE VIEW fullJoin AS
SELECT * FROM leftJoin
UNION
SELECT * FROM rightJoin;


