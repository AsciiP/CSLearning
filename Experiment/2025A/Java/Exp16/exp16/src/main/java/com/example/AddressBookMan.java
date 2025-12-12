package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.SQLIntegrityConstraintViolationException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class AddressBookMan {
    private static final String DB_URL = "jdbc:h2:mem:addressbook";
    private static final String DB_DRIVER = "org.h2.Driver";
    private Connection conn;
    private Scanner scanner;
    private int currentUserId = -1;

    public static void main(String[] args) {
        AddressBookMan app = new AddressBookMan();
        app.run();
    }

    private void run() {
        try {
            scanner = new Scanner(System.in);
            initDatabase();
            loginOrRegister();
            mainMenu();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            closeConnection();
        }
    }

    private void initDatabase() throws Exception {
        Class.forName(DB_DRIVER);
        conn = DriverManager.getConnection(DB_URL);
        
        try (Statement stmt = conn.createStatement()) {
            // 创建User表
            stmt.execute("CREATE TABLE IF NOT EXISTS \"user\" (" +
                    "id INT PRIMARY KEY AUTO_INCREMENT," +
                    "username VARCHAR(50) UNIQUE NOT NULL," +
                    "password VARCHAR(50) NOT NULL)");
            
            // 创建Group表
            stmt.execute("CREATE TABLE IF NOT EXISTS \"grp\" (" +
                    "id INT PRIMARY KEY AUTO_INCREMENT," +
                    "user_id INT NOT NULL," +
                    "name VARCHAR(50) NOT NULL," +
                    "FOREIGN KEY(user_id) REFERENCES \"user\"(id) ON DELETE CASCADE)");
            
            // 创建Communication表
            stmt.execute("CREATE TABLE IF NOT EXISTS \"communication\" (" +
                    "id INT PRIMARY KEY AUTO_INCREMENT," +
                    "user_id INT NOT NULL," +
                    "name VARCHAR(50) NOT NULL," +
                    "phone VARCHAR(20) NOT NULL," +
                    "email VARCHAR(50)," +
                    "group_id INT," +
                    "FOREIGN KEY(user_id) REFERENCES \"user\"(id) ON DELETE CASCADE," +
                    "FOREIGN KEY(group_id) REFERENCES \"grp\"(id) ON DELETE SET NULL)");
        }
    }

    private void loginOrRegister() throws SQLException {
        while (true) {
            System.out.println("\n========== 通讯录系统 ==========");
            System.out.println("1. 登录");
            System.out.println("2. 注册");
            System.out.print("请选择 (1-2): ");
            
            String choice = scanner.nextLine().trim();
            if ("1".equals(choice)) {
                if (login()) break;
            } else if ("2".equals(choice)) {
                register();
            } else {
                System.out.println("无效选择!");
            }
        }
    }

    private boolean login() throws SQLException {
        System.out.print("用户名: ");
        String username = scanner.nextLine().trim();
        System.out.print("密码: ");
        String password = scanner.nextLine().trim();
        
        try (PreparedStatement pstmt = conn.prepareStatement(
                "SELECT id FROM \"user\" WHERE username=? AND password=?")) {
            pstmt.setString(1, username);
            pstmt.setString(2, password);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    currentUserId = rs.getInt("id");
                    System.out.println("登录成功! 欢迎 " + username);
                    return true;
                } else {
                    System.out.println("用户名或密码错误!");
                    return false;
                }
            }
        }
    }

    private void register() throws SQLException {
        System.out.print("新用户名: ");
        String username = scanner.nextLine().trim();
        System.out.print("密码: ");
        String password = scanner.nextLine().trim();
        
        try (PreparedStatement pstmt = conn.prepareStatement(
                "INSERT INTO \"user\"(username, password) VALUES(?, ?)")) {
            pstmt.setString(1, username);
            pstmt.setString(2, password);
            pstmt.executeUpdate();
            System.out.println("注册成功! 请重新登录");
        } catch (SQLIntegrityConstraintViolationException e) {
            System.out.println("用户名已存在!");
        }
    }

    private void mainMenu() throws SQLException {
        while (true) {
            System.out.println("\n========== 主菜单 ==========");
            System.out.println("1. 显示通讯录");
            System.out.println("2. 添加联系人");
            System.out.println("3. 修改联系人");
            System.out.println("4. 删除联系人");
            System.out.println("5. 管理分组");
            System.out.println("6. 登出");
            System.out.print("请选择 (1-6): ");
            
            String choice = scanner.nextLine().trim();
            switch (choice) {
                case "1" -> displayContacts();
                case "2" -> addContact();
                case "3" -> modifyContact();
                case "4" -> deleteContact();
                case "5" -> manageGroups();
                case "6" -> {
                    currentUserId = -1;
                    return;
                }
                default -> System.out.println("无效选择!");
            }
        }
    }

    private void displayContacts() throws SQLException {
        try (PreparedStatement pstmt = conn.prepareStatement(
                "SELECT c.id, c.name, c.phone, c.email, g.name as group_name " +
                "FROM \"communication\" c LEFT JOIN \"grp\" g ON c.group_id=g.id " +
                "WHERE c.user_id=? ORDER BY c.id")) {
            pstmt.setInt(1, currentUserId);
            try (ResultSet rs = pstmt.executeQuery()) {
                System.out.println("\n========== 通讯录列表 ==========");
                System.out.printf("%-4s | %-12s | %-13s | %-20s | %-10s%n", 
                        "编号", "姓名", "电话", "邮箱", "分组");
                System.out.println("------+---------------+---------------+----------------------+----------");
                
                boolean hasData = false;
                while (rs.next()) {
                    hasData = true;
                    System.out.printf("%-4d | %-12s | %-13s | %-20s | %-10s%n",
                            rs.getInt("id"),
                            rs.getString("name"),
                            rs.getString("phone"),
                            rs.getString("email") == null ? "-" : rs.getString("email"),
                            rs.getString("group_name") == null ? "-" : rs.getString("group_name"));
                }
                if (!hasData) {
                    System.out.println("通讯录为空");
                }
            }
        }
    }

    private void addContact() throws SQLException {
        System.out.print("姓名: ");
        String name = scanner.nextLine().trim();
        System.out.print("电话: ");
        String phone = scanner.nextLine().trim();
        System.out.print("邮箱: ");
        String email = scanner.nextLine().trim();
        
        Integer groupId = selectGroup();
        
        try (PreparedStatement pstmt = conn.prepareStatement(
                "INSERT INTO \"communication\"(user_id, name, phone, email, group_id) VALUES(?, ?, ?, ?, ?)")) {
            pstmt.setInt(1, currentUserId);
            pstmt.setString(2, name);
            pstmt.setString(3, phone);
            pstmt.setString(4, email.isEmpty() ? null : email);
            pstmt.setObject(5, groupId);
            pstmt.executeUpdate();
            System.out.println("添加成功!");
        }
    }

    private void modifyContact() throws SQLException {
        System.out.print("输入要修改的联系人编号: ");
        try {
            int contactId = Integer.parseInt(scanner.nextLine().trim());
            
            System.out.print("新姓名 (按回车跳过): ");
            String name = scanner.nextLine().trim();
            System.out.print("新电话 (按回车跳过): ");
            String phone = scanner.nextLine().trim();
            System.out.print("新邮箱 (按回车跳过): ");
            String email = scanner.nextLine().trim();
            
            StringBuilder sql = new StringBuilder("UPDATE communication SET ");
            List<Object> params = new ArrayList<>();
            
            if (!name.isEmpty()) {
                sql.append("name=?");
                params.add(name);
            }
            if (!phone.isEmpty()) {
                if (!params.isEmpty()) sql.append(", ");
                sql.append("phone=?");
                params.add(phone);
            }
            if (!email.isEmpty()) {
                if (!params.isEmpty()) sql.append(", ");
                sql.append("email=?");
                params.add(email);
            }
            
            if (params.isEmpty()) {
                System.out.println("未修改任何内容");
                return;
            }
            
            sql.append(" WHERE id=? AND user_id=?");
            params.add(contactId);
            params.add(currentUserId);
            
            try (PreparedStatement pstmt = conn.prepareStatement(sql.toString())) {
                for (int i = 0; i < params.size(); i++) {
                    pstmt.setObject(i + 1, params.get(i));
                }
                int rows = pstmt.executeUpdate();
                System.out.println(rows > 0 ? "修改成功!" : "联系人不存在!");
            }
        } catch (NumberFormatException e) {
            System.out.println("编号格式错误!");
        }
    }

    private void deleteContact() throws SQLException {
        System.out.print("输入要删除的联系人编号: ");
        try {
            int contactId = Integer.parseInt(scanner.nextLine().trim());
            
            try (PreparedStatement pstmt = conn.prepareStatement(
                    "DELETE FROM \"communication\" WHERE id=? AND user_id=?")) {
                pstmt.setInt(1, contactId);
                pstmt.setInt(2, currentUserId);
                int rows = pstmt.executeUpdate();
                System.out.println(rows > 0 ? "删除成功!" : "联系人不存在!");
            }
        } catch (NumberFormatException e) {
            System.out.println("编号格式错误!");
        }
    }

    private void manageGroups() throws SQLException {
        while (true) {
            System.out.println("\n========== 分组管理 ==========");
            System.out.println("1. 查看分组");
            System.out.println("2. 添加分组");
            System.out.println("3. 删除分组");
            System.out.println("4. 返回");
            System.out.print("请选择 (1-4): ");
            
            String choice = scanner.nextLine().trim();
            switch (choice) {
                case "1" -> listGroups();
                case "2" -> addGroup();
                case "3" -> deleteGroup();
                case "4" -> {
                    return;
                }
                default -> System.out.println("无效选择!");
            }
        }
    }

    private void listGroups() throws SQLException {
        try (PreparedStatement pstmt = conn.prepareStatement(
                "SELECT id, name FROM \"grp\" WHERE user_id=? ORDER BY id")) {
            pstmt.setInt(1, currentUserId);
            try (ResultSet rs = pstmt.executeQuery()) {
                System.out.println("\n分组列表:");
                boolean hasData = false;
                while (rs.next()) {
                    hasData = true;
                    System.out.println(rs.getInt("id") + ". " + rs.getString("name"));
                }
                if (!hasData) {
                    System.out.println("暂无分组");
                }
            }
        }
    }

    private void addGroup() throws SQLException {
        System.out.print("分组名称: ");
        String groupName = scanner.nextLine().trim();
        
        try (PreparedStatement pstmt = conn.prepareStatement(
                "INSERT INTO \"grp\"(user_id, name) VALUES(?, ?)")) {
            pstmt.setInt(1, currentUserId);
            pstmt.setString(2, groupName);
            pstmt.executeUpdate();
            System.out.println("分组添加成功!");
        }
    }

    private void deleteGroup() throws SQLException {
        System.out.print("输入要删除的分组编号: ");
        try {
            int groupId = Integer.parseInt(scanner.nextLine().trim());
            
            try (PreparedStatement pstmt = conn.prepareStatement(
                    "DELETE FROM \"grp\" WHERE id=? AND user_id=?")) {
                pstmt.setInt(1, groupId);
                pstmt.setInt(2, currentUserId);
                int rows = pstmt.executeUpdate();
                System.out.println(rows > 0 ? "删除成功!" : "分组不存在!");
            }
        } catch (NumberFormatException e) {
            System.out.println("编号格式错误!");
        }
    }

    private Integer selectGroup() throws SQLException {
        listGroups();
        System.out.print("选择分组编号 (不选请按回车): ");
        String input = scanner.nextLine().trim();
        
        if (input.isEmpty()) {
            return null;
        }
        
        try {
            return Integer.parseInt(input);
        } catch (NumberFormatException e) {
            System.out.println("无效的分组编号，将不分配分组");
            return null;
        }
    }

    private void closeConnection() {
        try {
            if (conn != null && !conn.isClosed()) {
                conn.close();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}