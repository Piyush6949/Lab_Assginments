<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" import="java.sql.*" %>

<html>
<head>
    <title>EBook Shop</title>
</head>
<body>

<h2>EBook Shop Details</h2>

<table border="1">
<tr>
    <th>ID</th>
    <th>Title</th>
    <th>Author</th>
    <th>Price</th>
    <th>Quantity</th>
</tr>

<%
    try {
        // Load Driver
        Class.forName("com.mysql.cj.jdbc.Driver");

        // Connect DB
        Connection con = DriverManager.getConnection(
            "jdbc:mysql://10.10.8.119:3306/te31461_db", "te31461", "te31461");

        Statement stmt = con.createStatement();
        ResultSet rs = stmt.executeQuery("SELECT * FROM ebookshop");

        while(rs.next()) {
%>

<tr>
    <td><%= rs.getInt("book_id") %></td>
    <td><%= rs.getString("book_title") %></td>
    <td><%= rs.getString("book_author") %></td>
    <td><%= rs.getDouble("book_price") %></td>
    <td><%= rs.getInt("quantity") %></td>
</tr>

<%
        }

        con.close();
    } catch(Exception e) {
%>
        <h3>Error: <%= e.getMessage() %></h3>
<%
    }
%>

</table>

</body>
</html>