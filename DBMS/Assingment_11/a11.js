// Insert sample document
db.employees.insertOne({
    empId: 1,
    name: "Rahul Mehta",
    age: 30,
    department: "HR",
    salary: 45000
});

// Example data (optional - for better testing)
db.employees.insertMany([
    { empId: 2, name: "Neha Sharma", age: 28, department: "IT", salary: 60000 },
    { empId: 3, name: "Amit Patel", age: 35, department: "Finance", salary: 55000 },
    { empId: 4, name: "Sanya Kapoor", age: 29, department: "IT", salary: 62000 },
    { empId: 5, name: "Ravi Kumar", age: 40, department: "HR", salary: 48000 }
]);

// Map function -> emit department as key and salary as value
var mapFunction = function () {
    emit(this.department, this.salary);
};

// Reduce function -> sum salaries of each department
var reduceFunction = function (key, values) {
    return Array.sum(values);
};

// Run mapReduce
db.employees.mapReduce(
    mapFunction,
    reduceFunction,
    { out: "department_salary_totals" }
);

// Show results
db.department_salary_totals.find();
