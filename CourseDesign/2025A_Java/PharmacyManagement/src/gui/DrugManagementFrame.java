package gui;

import dao.DrugDAO;
import dao.CategoryDAO;
import dao.SupplierDAO;
import dao.StockHistoryDAO;
import model.Drug;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class DrugManagementFrame extends JFrame {
    
    private final DrugDAO drugDAO = new DrugDAO();
    private final CategoryDAO categoryDAO = new CategoryDAO();
    private final SupplierDAO supplierDAO = new SupplierDAO();
    private final StockHistoryDAO stockHistoryDAO = new StockHistoryDAO();
    
    private JTable drugTable;
    private DefaultTableModel tableModel;

    public DrugManagementFrame() {
        setTitle("药品管理");
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setSize(1000, 700);
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
        
        loadDrugData();
    }

    private JPanel createToolBar() {
        JPanel panel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        
        JButton addBtn = new JButton("添加药品");
        JButton editBtn = new JButton("编辑药品");
        JButton deleteBtn = new JButton("删除药品");
        JButton stockInBtn = new JButton("进货");
        JButton stockOutBtn = new JButton("出货");
        JButton categoryBtn = new JButton("管理分类");
        JButton refreshBtn = new JButton("刷新");

        addBtn.addActionListener(e -> showAddDrugDialog());
        editBtn.addActionListener(e -> showEditDrugDialog());
        deleteBtn.addActionListener(e -> deleteDrug());
        stockInBtn.addActionListener(e -> showStockInDialog());
        stockOutBtn.addActionListener(e -> showStockOutDialog());
        categoryBtn.addActionListener(e -> showCategoryDialog());
        refreshBtn.addActionListener(e -> loadDrugData());

        panel.add(addBtn);
        panel.add(editBtn);
        panel.add(deleteBtn);
        panel.add(new JSeparator(JSeparator.VERTICAL));
        panel.add(stockInBtn);
        panel.add(stockOutBtn);
        panel.add(new JSeparator(JSeparator.VERTICAL));
        panel.add(categoryBtn);
        panel.add(refreshBtn);

        return panel;
    }

    private JPanel createTablePanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        String[] columns = {"ID", "药品名称", "条形码", "分类名称", "供应商名称", "价格", "库存", "描述", "过期日期"};
        tableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        drugTable = new JTable(tableModel);
        drugTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        drugTable.setRowHeight(25);
        
        JScrollPane scrollPane = new JScrollPane(drugTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        return panel;
    }

    private void loadDrugData() {
        tableModel.setRowCount(0);
        List<Drug> drugs = drugDAO.findAll();
        for (Drug drug : drugs) {
            String categoryName = "未分类";
            String supplierName = "未指定";
            
            if (drug.getCategoryId() > 0) {
                var category = categoryDAO.findById(drug.getCategoryId());
                if (category != null) {
                    categoryName = category.getName();
                }
            }
            
            if (drug.getSupplierId() > 0) {
                var supplier = supplierDAO.findById(drug.getSupplierId());
                if (supplier != null) {
                    supplierName = supplier.getName();
                }
            }
            
            Object[] row = {
                drug.getId(),
                drug.getName(),
                drug.getBarcode() == null ? "" : drug.getBarcode(),
                categoryName,
                supplierName,
                drug.getPrice(),
                drug.getStock(),
                drug.getDescription(),
                drug.getExpireDate()
            };
            tableModel.addRow(row);
        }
    }

    private void showAddDrugDialog() {
        new AddDrugDialog(this, drugDAO, categoryDAO, supplierDAO) {
            @Override
            protected void onSuccess() {
                loadDrugData();
            }
        }.setVisible(true);
    }

    private void showEditDrugDialog() {
        int row = drugTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要编辑的药品！");
            return;
        }
        
        int drugId = (int) tableModel.getValueAt(row, 0);
        Drug drug = drugDAO.findById(drugId);
        
        new EditDrugDialog(this, drug, drugDAO, categoryDAO, supplierDAO) {
            @Override
            protected void onSuccess() {
                loadDrugData();
            }
        }.setVisible(true);
    }

    private void deleteDrug() {
        int row = drugTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要删除的药品！");
            return;
        }
        
        int result = JOptionPane.showConfirmDialog(this, "确定删除该药品吗？", "确认删除", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            int drugId = (int) tableModel.getValueAt(row, 0);
            drugDAO.delete(drugId);
            loadDrugData();
            JOptionPane.showMessageDialog(this, "删除成功！");
        }
    }

    private void showStockInDialog() {
        int row = drugTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要进货的药品！");
            return;
        }
        
        int drugId = (int) tableModel.getValueAt(row, 0);
        Drug drug = drugDAO.findById(drugId);
        
        new StockOperationDialog(this, drug, true, drugDAO, stockHistoryDAO) {
            @Override
            protected void onSuccess() {
                loadDrugData();
            }
        }.setVisible(true);
    }

    private void showStockOutDialog() {
        int row = drugTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要出货的药品！");
            return;
        }
        
        int drugId = (int) tableModel.getValueAt(row, 0);
        Drug drug = drugDAO.findById(drugId);
        
        new StockOperationDialog(this, drug, false, drugDAO, stockHistoryDAO) {
            @Override
            protected void onSuccess() {
                loadDrugData();
            }
        }.setVisible(true);
    }

    private void showCategoryDialog() {
        new CategoryDialog(this, categoryDAO).setVisible(true);
    }
}
