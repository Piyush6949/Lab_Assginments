// ASSIGNMENT 9

// use CompanyDB;

db.createCollection("employees");

db.employees.insertOne({
    empId: 1,
    name: "Rahul Mehta",
    age: 30,
    department: "HR",
    salary: 45000
});

db.employees.insertMany([
    {
        empId: 2,
        name: "Anita Sharma",
        age: 28,
        department: "IT",
        salary: 60000
    },
    {
        empId: 3,
        name: "Suresh Patel",
        age: 35,
        department: "Finance",
        salary: 55000
    },
    {
        empId: 4,
        name: "Priya Nair",
        age: 26,
        department: "IT",
        salary: 40000
    }
]);

db.employees.find()
db.employees.find().pretty()
db.employees.findOne()
db.employees.find({ salary: { $gt: 50000 } })
db.employees.find({ age: { $lt: 30 } })
db.employees.find({ department: "IT" })
db.employees.find({ $and: [{ salary: { $gt: 45000 } }, { age: { $lt: 32 } }] })

db.employees.updateOne(
    { empId: 1 },
    { $set: { salary: 48000 } }
)

// save method: replace existing document with new values
db.employees.save({
    _id: ObjectId("PUT_EXISTING_ID_HERE"),
    empId: 2,
    name: "Anita Sharma",
    age: 28,
    department: "IT",
    salary: 62000
})

db.employees.deleteOne({ empId: 4 })
db.employees.deleteMany({ department: "Finance" })
