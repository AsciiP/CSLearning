package gui;

import dao.SupplierDAO;

import javax.swing.*;
import java.awt.*;

public class AdminFrame extends JFrame {
    
    private final SupplierDAO supplierDAO = new SupplierDAO();

    public AdminFrame(String username) {
        setTitle("药店管理系统 - 管理员 - " + username);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        setLocationRelativeTo(null);
        setResizable(true);

        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));

        // 标题
        JLabel titleLabel = new JLabel("药店管理系统 - 主菜单", JLabel.CENTER);
        titleLabel.setFont(new Font("微软雅黑", Font.BOLD, 24));
        mainPanel.add(titleLabel, BorderLayout.NORTH);

        // 中心菜单面板
        JPanel menuPanel = createMenuPanel();
        mainPanel.add(menuPanel, BorderLayout.CENTER);

        // 底部退出按钮
        JPanel bottomPanel = createBottomPanel();
        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        setContentPane(mainPanel);
        setVisible(true);
    }


    private JPanel createMenuPanel() {
        JPanel panel = new JPanel(new GridLayout(2, 2, 20, 20));
        panel.setBorder(BorderFactory.createEmptyBorder(40, 80, 40, 80));

        // 创建四个菜单按钮
        JButton drugManagementBtn = createMenuButton("管理药品", "管理药品信息、分类等");
        JButton supplierManagementBtn = createMenuButton("管理供应商", "管理供应商信息");
        JButton stockManagementBtn = createMenuButton("查看进出货记录", "查看进出货历史记录");
        JButton reportBtn = createMenuButton("统计报表", "查看收银员业绩和药品销量统计");

        // 添加按钮事件
        drugManagementBtn.addActionListener(e -> openDrugManagement());
        supplierManagementBtn.addActionListener(e -> openSupplierManagement());
        stockManagementBtn.addActionListener(e -> openStockManagement());
        reportBtn.addActionListener(e -> openReport());

        panel.add(drugManagementBtn);
        panel.add(supplierManagementBtn);
        panel.add(stockManagementBtn);
        panel.add(reportBtn);

        return panel;
    }

    private JButton createMenuButton(String title, String description) {
        JButton button = new JButton();
        button.setLayout(new BorderLayout(5, 5));
        button.setPreferredSize(new Dimension(200, 120));
        button.setFont(new Font("微软雅黑", Font.PLAIN, 14));

        JLabel titleLabel = new JLabel(title, JLabel.CENTER);
        titleLabel.setFont(new Font("微软雅黑", Font.BOLD, 18));
        
        JLabel descLabel = new JLabel("<html><center>" + description + "</center></html>", JLabel.CENTER);
        descLabel.setFont(new Font("微软雅黑", Font.PLAIN, 12));
        descLabel.setForeground(Color.GRAY);

        JPanel textPanel = new JPanel(new GridLayout(2, 1, 5, 5));
        textPanel.setOpaque(false);
        textPanel.add(titleLabel);
        textPanel.add(descLabel);

        button.add(textPanel, BorderLayout.CENTER);
        
        return button;
    }

    private JPanel createBottomPanel() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.CENTER));
        panel.setBorder(BorderFactory.createEmptyBorder(10, 0, 10, 0));
        
        JButton logoutBtn = new JButton("退出登录");
        logoutBtn.setFont(new Font("微软雅黑", Font.PLAIN, 14));
        logoutBtn.addActionListener(e -> logout());
        
        panel.add(logoutBtn);
        
        return panel;
    }

    private void openDrugManagement() {
        new DrugManagementFrame();
    }

    private void openSupplierManagement() {
        new SupplierDialog(this, supplierDAO).setVisible(true);
    }

    private void openStockManagement() {
        new StockManagementFrame();
    }

    private void openReport() {
        new ReportFrame();
    }

    private void logout() {
        int result = JOptionPane.showConfirmDialog(this, "确定要退出登录吗？", "确认退出", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            this.dispose();
            new LoginFrame();
        }
    }
}
