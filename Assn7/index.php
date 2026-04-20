<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Ass7</title>
</head>
<h2>Add Book</h2>
<form action="add.php" method="post">
    Title: <input type="text" name="title"><br>
    Author: <input type="text" name="author"><br>
    Price: <input type="number" step="0.1" name="price"><br>
    Quantity: <input type="number" name="quantity"><br>
    <input type="submit" value="Add Book">
</form>

<br>

<h2>Update Book</h2>
<form action="update.php" method="post">
    Book ID: <input type="number" name="id"><br>
    New Price: <input type="number" step="0.1" name="price"><br>
    New Quantity: <input type="number" name="quantity"><br>
    <input type="submit" value="Update Book">
</form>

<br>

<h2>Delete Book</h2>
<form action="delete.php" method="get">
    Book ID: <input type="number" name="id"><br>
    <input type="submit" value="Delete Book">
</form>

<br>

<a href="view.php">View All Books</a>
<body>
</body>
</html>