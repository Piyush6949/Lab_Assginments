<?php
$conn = new mysqli("10.10.8.119", "te31461", "te31461", "te31461_db",3306); // change db name if needed

if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
?>