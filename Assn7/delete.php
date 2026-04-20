<?php
include 'db.php';

$id = $_GET['id'];

$sql = "DELETE FROM ebookshop WHERE book_id=$id";

$conn->query($sql);

echo "Book deleted successfully";
?>