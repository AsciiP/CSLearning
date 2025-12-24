package gui;

import util.DBUtil;
import util.PasswordUtil;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class LoginFrame extends JFrame {
    
    private final JTextField usernameField = new JTextField();
    private final JPasswordField passwordField = new JPasswordField();
    private final int TESTMODE = 0;  // 0: 关闭  1: 管理员  2: 用户

    @SuppressWarnings("unused")
    public LoginFrame() {
        setTitle("药店管理系统 - 登录");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(420, 260);
        setLocationRelativeTo(null);
        setResizable(false);

        JPanel panel = new JPanel(new GridBagLayout());
        panel.setBorder(BorderFactory.createEmptyBorder(16, 16, 16, 16));
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.anchor = GridBagConstraints.WEST;

        JLabel title = new JLabel("药店管理系统");
        title.setFont(new Font("Microsoft YaHei", Font.BOLD, 20));
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        panel.add(title, gbc);

        gbc.gridwidth = 1;
        gbc.anchor = GridBagConstraints.WEST;

        gbc.gridx = 0;
        gbc.gridy = 1;
        panel.add(new JLabel("用户名:"), gbc);
        gbc.gridx = 1;
        usernameField.setColumns(16);
        panel.add(usernameField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(new JLabel("密码:"), gbc);
        gbc.gridx = 1;
        passwordField.setColumns(16);
        panel.add(passwordField, gbc);

        JPanel btnPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 12, 0));
        JButton loginBtn = new JButton("登录");
        JButton openRegisterBtn = new JButton("注册");

        loginBtn.addActionListener(this::login);

        openRegisterBtn.addActionListener(e -> {
            new RegisterFrame(this).setVisible(true);
        });

        btnPanel.add(loginBtn);
        btnPanel.add(openRegisterBtn);

        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.gridwidth = 2;
        gbc.anchor = GridBagConstraints.CENTER;
        panel.add(btnPanel, gbc);

        setContentPane(panel);
        getRootPane().setDefaultButton(loginBtn);
        
        // TESTMODE
        if (TESTMODE == 1) {
            setVisible(false);
            new AdminFrame("testadmin");
            this.dispose();
        } else if (TESTMODE == 2) {
            setVisible(false);
            new UserFrame("testuser", 1);
            this.dispose();
        } else {
            setVisible(true);
        }
    }

    private void login(ActionEvent e) {
        String username = usernameField.getText();
        String password = new String(passwordField.getPassword());

        try (Connection conn = DBUtil.getConnection()) {
            String sql = "SELECT id, password, role FROM user WHERE username=?";
            PreparedStatement ps = conn.prepareStatement(sql);
            ps.setString(1, username);

            ResultSet rs = ps.executeQuery();
            if (rs.next()) {
                String stored = rs.getString("password");
                if (PasswordUtil.verify(password, stored)) {
                    JOptionPane.showMessageDialog(this, "登录成功！");
                    int userId = rs.getInt("id");
                    String role = rs.getString("role");
                    
                    if ("admin".equals(role)) {
                        new AdminFrame(username);
                    } else {
                        new UserFrame(username, userId);
                    }
                    
                    this.dispose();
                } else {
                    JOptionPane.showMessageDialog(this, "用户名或密码错误！");
                }
            } else {
                JOptionPane.showMessageDialog(this, "用户名或密码错误！");
            }
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }
}