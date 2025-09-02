CREATE TABLE Department (
  deptId INT PRIMARY KEY,
  deptName VARCHAR(100) NOT NULL,
  deptLocation VARCHAR(100)
);

CREATE TABLE Employee (
    empId INT PRIMARY KEY,
    deptId INT,
    empFname VARCHAR(100) NOT NULL, 
    empLname VARCHAR(100) NOT NULL,
    empPosition VARCHAR(100),       
    empSalary INT NOT NULL,
    empJoinDate DATE NOT NULL 
);

CREATE TABLE Project(
  projId INT PRIMARY KEY,
  deptId INT,
  projName VARCHAR(100) NOT NULL,
  projLocation VARCHAR(100),
  projCost INT NOT NULL DEFAULT 0,
  projYear DATE NOT NULL
);

INSERT INTO Employee3 (empId, deptId, empFname, empLname, empPosition, empSalary, empJoinDate) VALUES
(1, 101, 'Prateek','Chavan', 'Software Engineer', 75000, '2000-03-15'),
(2, 101, 'Harsh', 'Khilare','Data Scientist', 85000, '2002-07-22'),
(3, 102, 'Priya','Mishra', 'Network Administrator', 60000, '2005-11-01'),
(4, 102, 'Rahul','Tripathi', 'IT Support', 45000, '2008-01-10'),
(5, 103, 'Heena','Tomar', 'HR Manager', 90000, '1983-05-20'),
(6, 103, 'Preeti','Zinta', 'HR Assistant', 40000, '2010-09-01'),
(7, 104, 'Parth','Gonjari', 'Financial Analyst', 70000, '1980-02-28'),
(8, 104, 'Hitesh','Tiwari', 'Accountant', 55000, '2012-04-12'),
(9, 105, 'Pooja','Hedge', 'Marketing Specialist', 65000, '2015-06-01'),
(10, 105, 'Himanshu','Patil', 'Marketing Manager', 95000, '2018-08-01'),
(11, 101, 'Peter','John', 'Project Manager', 100000, '1984-11-11'),
(12, 102, 'Henry','Mark', 'System Analyst', 70000, '1990-03-25');

INSERT INTO Department (deptId, deptName, deptLocation) VALUES
(101, 'Computer', 'New York'),
(102, 'IT', 'London'),
(103, 'HR', 'Paris'),
(104, 'Finance', 'Pune'),
(105, 'Marketing', 'Sydney'),
(106, 'Operations', 'Berlin'),
(107, 'Research', 'Mumbai'),
(108, 'Sales', 'Dubai'),
(109, 'Legal', 'Rome'),
(110, 'Admin', 'Banglore');
SELECT Employee.empId,Department.deptName FROM Employee LEFT JOIN Department ON Employee.deptId = Department.deptId; 

INSERT INTO Project (projId, deptId, projName, projLocation, projCost, projYear) VALUES
(1001, 101, 'Website Redesign', 'Pune', 150000, '2004-01-01'),
(1002, 101, 'Database Migration', 'Bangalore', 300000, '2005-06-15'),
(1003, 102, 'Network Upgrade', 'Pune', 80000, '2006-03-20'),
(1004, 102, 'Security Audit', 'Hyderabad', 120000, '2007-09-10'),
(1005, 103, 'Recruitment Drive', 'Mumbai', 50000, '2008-02-01'),
(1006, 104, 'Annual Budgeting', 'Delhi', 200000, '2009-04-01'),
(1007, 105, 'New Product Launch', 'Pune', 450000, '2010-07-01'),
(1008, 101, 'Cloud Infrastructure', 'Chennai', 600000, '2011-01-01'),
(1009, 102, 'Software Development', 'Pune', 250000, '2004-11-01'),
(1010, 103, 'Employee Training', 'Kolkata', 75000, '2005-05-01'),
(1011, 101, 'Mobile App Development', 'Pune', 180000, '2007-03-01'),
(1012, 104, 'Financial Reporting System', 'Ahmedabad', 350000, '2007-08-01');


--2
--Display all Employee details with Department ‘Computer’ and ‘IT’ and Employee
first name starting with 'p' or 'h'.
SELECT * FROM Employee WHERE (deptId = 101 OR deptId = 102) AND (empFname LIKE 'P%' OR empFname LIKE 'H%') ;
--change for case sensitive

--3
--lists the number of different Employee Positions.
SELECT DISTINCT empPosition FROM Employee;
SELECT COUNT(DISTINCT empPosition) FROM Employee;
-- use group by

--4
--Give 10% increase in Salary of the Employee whose joining year is before 1985.
UPDATE Employee 
SET empSalary = empSalary + (empSalary*0.1)
WHERE empJoinDate < 1985

--5
--Delete Department details which location is ‘Mumbai’.
DELETE FROM Department WHERE deptLocation = 'Mumbai';

--6
--Find the names of Projects with location ‘pune’ .
SELECT projName FROM Project WHERE projLocation = 'Pune';

--7
--Find the project having cost in between 100000 to 500000.
SELECT * FROM Project WHERE projCost >= 100000 AND projCost <= 500000;
-- USE BETWEEN

--8
--Find the project having maximum price and find average of Project cost
SELECT *
FROM Project
WHERE projCost = (SELECT MIN(projCost) FROM Project;); 

SELECT AVG(projCost) FROM Project;

--9
--Display all employees with Emp _id and Emp name in decreasing order of Emp_lname
SELECT empId,empFname FROM Employee
ORDER BY empLname DESC;

--10
--Display Proj_name,Proj_location ,Proj_cost of all project started in 2004,2005,2007
SELECT projName, projLocation, projCost FROM Project WHERE (YEAR(projYear) IN (2004,2005,2007));



