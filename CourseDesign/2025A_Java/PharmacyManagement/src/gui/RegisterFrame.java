package gui;

import util.DBUtil;
import util.PasswordUtil;

import javax.swing.*;
import java.awt.*;
import java.sql.Connection;
import java.sql.PreparedStatement;

public class RegisterFrame extends JDialog {
    
    public RegisterFrame(JFrame parent) {
        super(parent, "用户注册", true);
        setSize(380, 320);
        setLocationRelativeTo(parent);

        JPanel rp = new JPanel(new GridBagLayout());
        rp.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        GridBagConstraints r = new GridBagConstraints();
        r.insets = new Insets(8, 8, 8, 8);
        r.anchor = GridBagConstraints.WEST;

        r.gridx = 0;
        r.gridy = 0;
        rp.add(new JLabel("用户名:"), r);
        JTextField regUser = new JTextField(16);
        r.gridx = 1;
        rp.add(regUser, r);

        r.gridx = 0;
        r.gridy = 1;
        rp.add(new JLabel("密码:"), r);
        JPasswordField regPass = new JPasswordField(16);
        r.gridx = 1;
        rp.add(regPass, r);

        r.gridx = 0;
        r.gridy = 2;
        rp.add(new JLabel("确认密码:"), r);
        JPasswordField regPass2 = new JPasswordField(16);
        r.gridx = 1;
        rp.add(regPass2, r);

        r.gridx = 0;
        r.gridy = 3;
        rp.add(new JLabel("权限:"), r);
        JComboBox<String> roleBox = new JComboBox<>(new String[] { "user", "admin" });
        r.gridx = 1;
        rp.add(roleBox, r);

        JPanel rbtn = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 0));
        JButton rCreate = new JButton("注册");
        JButton rCancel = new JButton("取消");

        rCreate.addActionListener(ae -> {
            String u = regUser.getText().trim();
            String p1 = new String(regPass.getPassword());
            String p2 = new String(regPass2.getPassword());
            String role = (String) roleBox.getSelectedItem();

            if (u.isEmpty() || p1.isEmpty()) {
                JOptionPane.showMessageDialog(RegisterFrame.this, "用户名与密码不能为空！");
                return;
            }
            if (!p1.equals(p2)) {
                JOptionPane.showMessageDialog(RegisterFrame.this, "两次密码不一致！");
                return;
            }

            try (Connection conn = DBUtil.getConnection()) {
                String hashed = PasswordUtil.generateSaltedHash(p1);
                String sql = "INSERT INTO user(username, password, role) VALUES(?, ?, ?)";
                PreparedStatement ps = conn.prepareStatement(sql);
                ps.setString(1, u);
                ps.setString(2, hashed);
                ps.setString(3, role);
                ps.executeUpdate();
                JOptionPane.showMessageDialog(RegisterFrame.this, "注册成功！");
                RegisterFrame.this.dispose();
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(RegisterFrame.this, "注册失败，可能用户名已存在或数据库错误！");
            }
        });

        rCancel.addActionListener(ae -> RegisterFrame.this.dispose());
        rbtn.add(rCreate);
        rbtn.add(rCancel);

        r.gridx = 0;
        r.gridy = 4;
        r.gridwidth = 2;
        r.anchor = GridBagConstraints.CENTER;
        rp.add(rbtn, r);

        setContentPane(rp);
        setResizable(false);
    }
}
