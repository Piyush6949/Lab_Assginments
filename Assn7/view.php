<?php
include 'db.php';

$result = $conn->query("SELECT * FROM ebookshop");

while($row = $result->fetch_assoc()) {
    echo "ID: ".$row['book_id']." | ".
         "Title: ".$row['book_title']." | ".
         "Author: ".$row['book_author']." | ".
         "Price: ".$row['book_price']." | ".
         "Qty: ".$row['quantity']."<br>";
}
?>