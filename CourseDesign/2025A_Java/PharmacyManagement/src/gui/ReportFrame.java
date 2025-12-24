package gui;

import dao.SalesDAO;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.math.BigDecimal;
import java.util.*;
import java.util.List;

public class ReportFrame extends JFrame {
    
    private final SalesDAO salesDAO = new SalesDAO();
    private JTable userTable;
    private DefaultTableModel userTableModel;
    private JTable drugTable;
    private DefaultTableModel drugTableModel;
    private JPanel userChartPanel;
    private JPanel drugChartPanel;

    public ReportFrame() {
        setTitle("统计报表");
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setSize(1000, 700);
        setLocationRelativeTo(null);
        setResizable(true);

        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // 创建选项卡
        JTabbedPane tabbedPane = new JTabbedPane();
        
        // 用户业绩统计选项卡
        JPanel userPanel = createUserPerformancePanel();
        tabbedPane.addTab("收银员业绩统计", userPanel);
        
        // 药品销量统计选项卡
        JPanel drugPanel = createDrugSalesPanel();
        tabbedPane.addTab("药品销量统计", drugPanel);
        
        mainPanel.add(tabbedPane, BorderLayout.CENTER);
        
        // 底部按钮
        JPanel bottomPanel = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JButton refreshBtn = new JButton("刷新数据");
        JButton closeBtn = new JButton("关闭");
        
        refreshBtn.addActionListener(e -> refreshData());
        closeBtn.addActionListener(e -> dispose());
        
        bottomPanel.add(refreshBtn);
        bottomPanel.add(closeBtn);
        mainPanel.add(bottomPanel, BorderLayout.SOUTH);

        setContentPane(mainPanel);
        
        // 加载数据
        loadUserPerformance();
        loadDrugSales();
        
        setVisible(true);
    }

    private JPanel createUserPerformancePanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        
        // 表格
        String[] columns = {"收银员", "销售笔数", "总销售额 (¥)"};
        userTableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        userTable = new JTable(userTableModel);
        userTable.setRowHeight(25);
        userTable.setFont(new Font("Microsoft YaHei", Font.PLAIN, 13));
        userTable.getTableHeader().setFont(new Font("Microsoft YaHei", Font.BOLD, 13));
        
        JScrollPane scrollPane = new JScrollPane(userTable);
        scrollPane.setPreferredSize(new Dimension(0, 250));
        panel.add(scrollPane, BorderLayout.NORTH);
        
        // 图表区域
        userChartPanel = new JPanel();
        userChartPanel.setLayout(new BorderLayout());
        userChartPanel.setBorder(BorderFactory.createTitledBorder("销售额对比图"));
        userChartPanel.setBackground(Color.WHITE);
        
        panel.add(userChartPanel, BorderLayout.CENTER);
        
        return panel;
    }

    private JPanel createDrugSalesPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        
        // 表格
        String[] columns = {"药品名称", "销售数量", "销售额 (¥)"};
        drugTableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        drugTable = new JTable(drugTableModel);
        drugTable.setRowHeight(25);
        drugTable.setFont(new Font("Microsoft YaHei", Font.PLAIN, 13));
        drugTable.getTableHeader().setFont(new Font("Microsoft YaHei", Font.BOLD, 13));
        
        JScrollPane scrollPane = new JScrollPane(drugTable);
        scrollPane.setPreferredSize(new Dimension(0, 250));
        panel.add(scrollPane, BorderLayout.NORTH);
        
        // 图表区域
        drugChartPanel = new JPanel();
        drugChartPanel.setLayout(new BorderLayout());
        drugChartPanel.setBorder(BorderFactory.createTitledBorder("销量对比图 (Top 10)"));
        drugChartPanel.setBackground(Color.WHITE);
        
        panel.add(drugChartPanel, BorderLayout.CENTER);
        
        return panel;
    }

    private void loadUserPerformance() {
        userTableModel.setRowCount(0);
        Map<String, Object[]> stats = salesDAO.getUserPerformance();
        
        // 按销售额排序
        List<Map.Entry<String, Object[]>> list = new ArrayList<>(stats.entrySet());
        list.sort((a, b) -> {
            BigDecimal totalA = (BigDecimal) a.getValue()[1];
            BigDecimal totalB = (BigDecimal) b.getValue()[1];
            return totalB.compareTo(totalA);
        });
        
        // 填充表格
        for (Map.Entry<String, Object[]> entry : list) {
            String username = entry.getKey();
            int count = (int) entry.getValue()[0];
            BigDecimal total = (BigDecimal) entry.getValue()[1];
            
            userTableModel.addRow(new Object[]{
                username,
                count,
                String.format("%.2f", total)
            });
        }
        
        // 绘制图表
        drawUserChart(list);
    }

    private void loadDrugSales() {
        drugTableModel.setRowCount(0);
        Map<String, Object[]> stats = salesDAO.getDrugSalesStats();
        
        // 按销量排序
        List<Map.Entry<String, Object[]>> list = new ArrayList<>(stats.entrySet());
        list.sort((a, b) -> {
            int qtyA = (int) a.getValue()[0];
            int qtyB = (int) b.getValue()[0];
            return Integer.compare(qtyB, qtyA);
        });
        
        // 填充表格
        for (Map.Entry<String, Object[]> entry : list) {
            String drugName = entry.getKey();
            int qty = (int) entry.getValue()[0];
            BigDecimal total = (BigDecimal) entry.getValue()[1];
            
            drugTableModel.addRow(new Object[]{
                drugName,
                qty,
                String.format("%.2f", total)
            });
        }
        
        // 绘制图表
        drawDrugChart(list);
    }

    private void drawUserChart(List<Map.Entry<String, Object[]>> data) {
        userChartPanel.removeAll();
        
        if (data.isEmpty()) {
            JLabel noDataLabel = new JLabel("暂无数据", JLabel.CENTER);
            noDataLabel.setFont(new Font("Microsoft YaHei", Font.PLAIN, 14));
            noDataLabel.setForeground(Color.GRAY);
            userChartPanel.add(noDataLabel, BorderLayout.CENTER);
            userChartPanel.revalidate();
            userChartPanel.repaint();
            return;
        }
        
        BarChartPanel chart = new BarChartPanel(data, true);
        userChartPanel.add(chart, BorderLayout.CENTER);
        userChartPanel.revalidate();
        userChartPanel.repaint();
    }

    private void drawDrugChart(List<Map.Entry<String, Object[]>> data) {
        drugChartPanel.removeAll();
        
        if (data.isEmpty()) {
            JLabel noDataLabel = new JLabel("暂无数据", JLabel.CENTER);
            noDataLabel.setFont(new Font("Microsoft YaHei", Font.PLAIN, 14));
            noDataLabel.setForeground(Color.GRAY);
            drugChartPanel.add(noDataLabel, BorderLayout.CENTER);
            drugChartPanel.revalidate();
            drugChartPanel.repaint();
            return;
        }
        
        // 只取前10名
        List<Map.Entry<String, Object[]>> top10 = data.size() > 10 ? data.subList(0, 10) : data;
        
        BarChartPanel chart = new BarChartPanel(top10, false);
        drugChartPanel.add(chart, BorderLayout.CENTER);
        drugChartPanel.revalidate();
        drugChartPanel.repaint();
    }

    private void refreshData() {
        loadUserPerformance();
        loadDrugSales();
        JOptionPane.showMessageDialog(this, "数据已刷新！");
    }

    /**
     * 简单的柱状图组件
     */
    private static class BarChartPanel extends JPanel {
        private final List<Map.Entry<String, Object[]>> data;
        private final boolean isSalesAmount; // true=销售额, false=销量
        
        public BarChartPanel(List<Map.Entry<String, Object[]>> data, boolean isSalesAmount) {
            this.data = data;
            this.isSalesAmount = isSalesAmount;
            setBackground(Color.WHITE);
        }
        
        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            
            if (data == null || data.isEmpty()) {
                return;
            }
            
            Graphics2D g2d = (Graphics2D) g;
            g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
            
            int width = getWidth();
            int height = getHeight();
            int padding = 40;
            int chartHeight = height - 2 * padding;
            int chartWidth = width - 2 * padding;
            
            // 找到最大值
            double maxValue = 0;
            for (Map.Entry<String, Object[]> entry : data) {
                double value;
                if (isSalesAmount) {
                    value = ((BigDecimal) entry.getValue()[1]).doubleValue();
                } else {
                    value = (int) entry.getValue()[0];
                }
                maxValue = Math.max(maxValue, value);
            }
            
            if (maxValue == 0) {
                maxValue = 1; // 避免除以0
            }
            
            // 绘制坐标轴
            g2d.setColor(Color.BLACK);
            g2d.drawLine(padding, padding, padding, height - padding); // Y轴
            g2d.drawLine(padding, height - padding, width - padding, height - padding); // X轴
            
            // 绘制柱状图
            int barWidth = Math.min(60, chartWidth / data.size() - 10);
            int spacing = (chartWidth - barWidth * data.size()) / (data.size() + 1);
            
            Color[] colors = {
                new Color(66, 133, 244),
                new Color(52, 168, 83),
                new Color(251, 188, 5),
                new Color(234, 67, 53),
                new Color(156, 39, 176)
            };
            
            for (int i = 0; i < data.size(); i++) {
                Map.Entry<String, Object[]> entry = data.get(i);
                String name = entry.getKey();
                double value;
                
                if (isSalesAmount) {
                    value = ((BigDecimal) entry.getValue()[1]).doubleValue();
                } else {
                    value = (int) entry.getValue()[0];
                }
                
                int barHeight = (int) ((value / maxValue) * chartHeight);
                int x = padding + spacing + i * (barWidth + spacing);
                int y = height - padding - barHeight;
                
                // 绘制柱子
                g2d.setColor(colors[i % colors.length]);
                g2d.fillRect(x, y, barWidth, barHeight);
                
                // 绘制边框
                g2d.setColor(Color.BLACK);
                g2d.drawRect(x, y, barWidth, barHeight);
                
                // 绘制数值
                g2d.setFont(new Font("Microsoft YaHei", Font.BOLD, 11));
                String valueStr = isSalesAmount ? String.format("%.0f", value) : String.valueOf((int) value);
                FontMetrics fm = g2d.getFontMetrics();
                int strWidth = fm.stringWidth(valueStr);
                g2d.drawString(valueStr, x + (barWidth - strWidth) / 2, y - 5);
                
                // 绘制标签（旋转显示）
                g2d.setFont(new Font("Microsoft YaHei", Font.PLAIN, 10));
                g2d.rotate(-Math.PI / 4, x + barWidth / 2, height - padding + 15);
                
                // 截断过长的名称
                String displayName = name.length() > 8 ? name.substring(0, 7) + "..." : name;
                g2d.drawString(displayName, x + barWidth / 2, height - padding + 15);
                
                g2d.rotate(Math.PI / 4, x + barWidth / 2, height - padding + 15);
            }
            
            // 绘制Y轴标签
            g2d.setFont(new Font("Microsoft YaHei", Font.PLAIN, 11));
            String yLabel = isSalesAmount ? "销售额 (元)" : "销量";
            g2d.drawString(yLabel, 5, padding - 10);
        }
    }
}
