// Count employees per department (sorted by count)
db.employees.aggregate([
  { $group: { _id: "$department", count: { $sum: 1 } } },
  { $sort: { count: -1 } }
]);

// Average salary per department (rounded to 2 decimals)
db.employees.aggregate([
  { $group: { _id: "$department", avgSalary: { $avg: "$salary" } } },
  { $project: { department: "$_id", avgSalary: { $round: ["$avgSalary", 2] }, _id: 0 } }
]);


// Create ascending index on name
db.employees.createIndex({ name: 1 });

// Show all indexes
db.employees.getIndexes();

// Create text index on name and department
db.employees.createIndex({ name: "text", department: "text" });

// Usage of text search
db.employees.find({ $text: { $search: "Rahul HR" } });

// Drop index by key spec
db.employees.dropIndex({ name: 1 });

// Drop index by index name
db.employees.dropIndex("name_1");
