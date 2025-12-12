package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class DatabaseDemo {
    public static void main(String[] args) {
        String url = "jdbc:sqlite:exp15_demo.db"; // local SQLite file
        try (Connection conn = DriverManager.getConnection(url)) {
            if (conn == null) {
                System.out.println("Failed to establish connection.");
                return;
            }

            try (Statement stmt = conn.createStatement()) {
                // 1. Create table
                String sqlCreate = "CREATE TABLE IF NOT EXISTS students (" +
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, " +
                        "name TEXT NOT NULL, " +
                        "age INTEGER, " +
                        "major TEXT)";
                System.out.println("SQL: " + sqlCreate);
                stmt.execute(sqlCreate);

                // 2. Insert rows
                String sqlInsert1 = "INSERT INTO students (name, age, major) VALUES ('Alice', 20, 'CS')";
                String sqlInsert2 = "INSERT INTO students (name, age, major) VALUES ('Bob', 22, 'Math')";
                System.out.println("SQL: " + sqlInsert1);
                stmt.executeUpdate(sqlInsert1);
                System.out.println("SQL: " + sqlInsert2);
                stmt.executeUpdate(sqlInsert2);

                // Parameterized insert example
                String sqlInsertParam = "INSERT INTO students (name, age, major) VALUES (?, ?, ?)";
                System.out.println("SQL (prepared): " + sqlInsertParam + " -> (Charlie, 21, Physics)");
                try (PreparedStatement ps = conn.prepareStatement(sqlInsertParam)) {
                    ps.setString(1, "Charlie");
                    ps.setInt(2, 21);
                    ps.setString(3, "Physics");
                    ps.executeUpdate();
                }

                // 5. Select - initial data
                System.out.println("\n-- Initial SELECT --");
                selectAll(conn);

                // 4. Update
                String sqlUpdate = "UPDATE students SET age = 23 WHERE name = 'Bob'";
                System.out.println("\nSQL: " + sqlUpdate);
                int updated = stmt.executeUpdate(sqlUpdate);
                System.out.println("Updated rows: " + updated);

                // Select after update
                System.out.println("\n-- SELECT after UPDATE --");
                selectAll(conn);

                // 3. Delete
                String sqlDelete = "DELETE FROM students WHERE name = 'Alice'";
                System.out.println("\nSQL: " + sqlDelete);
                int deleted = stmt.executeUpdate(sqlDelete);
                System.out.println("Deleted rows: " + deleted);

                // Select after delete
                System.out.println("\n-- SELECT after DELETE --");
                selectAll(conn);

                // Optional: show final SQL statements executed for lab record
                System.out.println("\nExecuted SQL statements:");
                System.out.println(sqlCreate);
                System.out.println(sqlInsert1);
                System.out.println(sqlInsert2);
                System.out.println(sqlInsertParam + " -> (Charlie, 21, Physics)");
                System.out.println(sqlUpdate);
                System.out.println(sqlDelete);
            }
        } catch (SQLException e) {
            System.err.println("Database error: " + e.getMessage());
            e.printStackTrace();
        }
    }

    private static void selectAll(Connection conn) throws SQLException {
        String sqlSelect = "SELECT id, name, age, major FROM students";
        System.out.println("SQL: " + sqlSelect);
        try (Statement s = conn.createStatement();
             ResultSet rs = s.executeQuery(sqlSelect)) {
            boolean any = false;
            while (rs.next()) {
                any = true;
                int id = rs.getInt("id");
                String name = rs.getString("name");
                int age = rs.getInt("age");
                String major = rs.getString("major");
                System.out.printf("id=%d, name=%s, age=%d, major=%s%n", id, name, age, major);
            }
            if (!any) {
                System.out.println("(no rows)");
            }
        }
    }
}