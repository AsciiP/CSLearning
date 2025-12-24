package gui;

import dao.StockHistoryDAO;
import dao.DrugDAO;
import model.StockHistory;
import model.Drug;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class StockManagementFrame extends JFrame {
    
    private final StockHistoryDAO stockHistoryDAO = new StockHistoryDAO();
    private final DrugDAO drugDAO = new DrugDAO();
    
    private JTable historyTable;
    private DefaultTableModel tableModel;
    private JComboBox<String> filterComboBox;

    public StockManagementFrame() {
        setTitle("进出货管理");
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setSize(900, 600);
        setLocationRelativeTo(null);
        setResizable(true);

        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // 顶部工具栏
        JPanel toolBar = createToolBar();
        mainPanel.add(toolBar, BorderLayout.NORTH);

        // 中间表格
        JPanel tablePanel = createTablePanel();
        mainPanel.add(tablePanel, BorderLayout.CENTER);

        setContentPane(mainPanel);
        setVisible(true);
        
        loadAllHistory();
    }

    private JPanel createToolBar() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        
        JLabel filterLabel = new JLabel("筛选：");
        filterComboBox = new JComboBox<>(new String[]{"全部记录", "仅进货", "仅出货"});
        JButton refreshBtn = new JButton("刷新");

        filterComboBox.addActionListener(e -> filterHistory());
        refreshBtn.addActionListener(e -> loadAllHistory());

        panel.add(filterLabel);
        panel.add(filterComboBox);
        panel.add(refreshBtn);

        return panel;
    }

    private JPanel createTablePanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        String[] columns = {"ID", "药品ID", "药品名称", "变动数量", "操作类型", "备注信息", "操作时间"};
        tableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        historyTable = new JTable(tableModel);
        historyTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        historyTable.setRowHeight(25);
        
        JScrollPane scrollPane = new JScrollPane(historyTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        return panel;
    }

    private void loadAllHistory() {
        tableModel.setRowCount(0);
        
        // 获取所有药品的历史记录
        List<Drug> drugs = drugDAO.findAll();
        for (Drug drug : drugs) {
            List<StockHistory> histories = stockHistoryDAO.findByDrugId(drug.getId());
            for (StockHistory history : histories) {
                addHistoryRow(history, drug.getName());
            }
        }
    }

    private void filterHistory() {
        tableModel.setRowCount(0);
        
        int selectedIndex = filterComboBox.getSelectedIndex();
        List<Drug> drugs = drugDAO.findAll();
        
        for (Drug drug : drugs) {
            List<StockHistory> histories = stockHistoryDAO.findByDrugId(drug.getId());
            for (StockHistory history : histories) {
                int change = history.getChangeAmount();
                
                // 根据筛选条件过滤
                if (selectedIndex == 0) { // 全部
                    addHistoryRow(history, drug.getName());
                } else if (selectedIndex == 1 && change > 0) { // 仅进货
                    addHistoryRow(history, drug.getName());
                } else if (selectedIndex == 2 && change < 0) { // 仅出货
                    addHistoryRow(history, drug.getName());
                }
            }
        }
    }

    private void addHistoryRow(StockHistory history, String drugName) {
        int change = history.getChangeAmount();
        String operationType = change > 0 ? "进货 +" + change : "出货 " + change;
        
        Object[] row = {
            history.getId(),
            history.getDrugId(),
            drugName,
            history.getChangeAmount(),
            operationType,
            history.getInfo() == null ? "" : history.getInfo(),
            history.getCreateTime()
        };
        tableModel.addRow(row);
    }
}
