<?php
include 'db.php';

$id = $_POST['id'];
$price = $_POST['price'];
$quantity = $_POST['quantity'];

$sql = "UPDATE ebookshop 
        SET book_price=$price, quantity=$quantity 
        WHERE book_id=$id";

$conn->query($sql);

echo "Book updated successfully";
?>