<?php
include 'db.php';

$title = $_POST['title'];
$author = $_POST['author'];
$price = $_POST['price'];
$quantity = $_POST['quantity'];

$sql = "INSERT INTO ebookshop (book_title, book_author, book_price, quantity)
        VALUES ('$title', '$author', $price, $quantity)";

$conn->query($sql);

echo "Book added successfully";
?>