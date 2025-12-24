package gui;

import dao.CategoryDAO;
import model.Category;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class CategoryDialog extends JDialog {
    private final CategoryDAO categoryDAO;
    private JTable categoryTable;
    private DefaultTableModel tableModel;

    public CategoryDialog(JFrame parent, CategoryDAO categoryDAO) {
        super(parent, "管理分类", true);
        this.categoryDAO = categoryDAO;

        setSize(500, 400);
        setLocationRelativeTo(parent);
        initComponents();
        loadData();
    }

    private void initComponents() {
        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel toolBar = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JButton addBtn = new JButton("添加");
        JButton deleteBtn = new JButton("删除");
        addBtn.addActionListener(e -> showAddDialog());
        deleteBtn.addActionListener(e -> deleteCategory());
        toolBar.add(addBtn);
        toolBar.add(deleteBtn);

        mainPanel.add(toolBar, BorderLayout.NORTH);

        String[] columns = {"ID", "分类名称"};
        tableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        categoryTable = new JTable(tableModel);
        categoryTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        categoryTable.setRowHeight(25);

        JScrollPane scrollPane = new JScrollPane(categoryTable);
        mainPanel.add(scrollPane, BorderLayout.CENTER);

        setContentPane(mainPanel);
    }

    private void loadData() {
        tableModel.setRowCount(0);
        List<Category> categories = categoryDAO.findAll();
        for (Category c : categories) {
            tableModel.addRow(new Object[]{c.getId(), c.getName()});
        }
    }

    private void showAddDialog() {
        String name = JOptionPane.showInputDialog(this, "输入分类名称:");
        if (name != null && !name.trim().isEmpty()) {
            categoryDAO.insert(new Category(name.trim()));
            loadData();
            JOptionPane.showMessageDialog(this, "添加成功！");
        }
    }

    private void deleteCategory() {
        int row = categoryTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要删除的分类！");
            return;
        }

        int result = JOptionPane.showConfirmDialog(this, "确定删除该分类吗？", "确认删除", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            int id = (int) tableModel.getValueAt(row, 0);
            categoryDAO.delete(id);
            loadData();
            JOptionPane.showMessageDialog(this, "删除成功！");
        }
    }
}
