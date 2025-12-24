package gui;

import dao.SupplierDAO;
import model.Supplier;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.util.List;

public class SupplierDialog extends JDialog {
    private final SupplierDAO supplierDAO;
    private JTable supplierTable;
    private DefaultTableModel tableModel;

    public SupplierDialog(JFrame parent, SupplierDAO supplierDAO) {
        super(parent, "管理供应商", true);
        this.supplierDAO = supplierDAO;

        setSize(600, 400);
        setLocationRelativeTo(parent);
        initComponents();
        loadData();
    }

    private void initComponents() {
        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        JPanel toolBar = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JButton addBtn = new JButton("添加");
        JButton editBtn = new JButton("编辑");
        JButton deleteBtn = new JButton("删除");
        addBtn.addActionListener(e -> showAddDialog());
        editBtn.addActionListener(e -> showEditDialog());
        deleteBtn.addActionListener(e -> deleteSupplier());
        toolBar.add(addBtn);
        toolBar.add(editBtn);
        toolBar.add(deleteBtn);

        mainPanel.add(toolBar, BorderLayout.NORTH);

        String[] columns = {"ID", "供应商名称", "电话", "地址"};
        tableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };

        supplierTable = new JTable(tableModel);
        supplierTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        supplierTable.setRowHeight(25);

        JScrollPane scrollPane = new JScrollPane(supplierTable);
        mainPanel.add(scrollPane, BorderLayout.CENTER);

        setContentPane(mainPanel);
    }

    private void loadData() {
        tableModel.setRowCount(0);
        List<Supplier> suppliers = supplierDAO.findAll();
        for (Supplier s : suppliers) {
            tableModel.addRow(new Object[]{s.getId(), s.getName(), s.getPhone(), s.getAddress()});
        }
    }

    private void showAddDialog() {
        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        JTextField nameField = new JTextField(20);
        JTextField phoneField = new JTextField(20);
        JTextField addressField = new JTextField(20);

        gbc.gridx = 0;
        gbc.gridy = 0;
        panel.add(new JLabel("供应商名称:"), gbc);
        gbc.gridx = 1;
        panel.add(nameField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        panel.add(new JLabel("电话:"), gbc);
        gbc.gridx = 1;
        panel.add(phoneField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(new JLabel("地址:"), gbc);
        gbc.gridx = 1;
        panel.add(addressField, gbc);

        int result = JOptionPane.showConfirmDialog(this, panel, "添加供应商", JOptionPane.OK_CANCEL_OPTION);
        if (result == JOptionPane.OK_OPTION) {
            String name = nameField.getText().trim();
            String phone = phoneField.getText().trim();
            String address = addressField.getText().trim();

            if (name.isEmpty()) {
                JOptionPane.showMessageDialog(this, "供应商名称不能为空！");
                return;
            }

            supplierDAO.insert(new Supplier(name, phone, address));
            loadData();
            JOptionPane.showMessageDialog(this, "添加成功！");
        }
    }

    private void showEditDialog() {
        int row = supplierTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要编辑的供应商！");
            return;
        }

        int id = (int) tableModel.getValueAt(row, 0);
        Supplier supplier = supplierDAO.findById(id);

        JPanel panel = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(5, 5, 5, 5);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        JTextField nameField = new JTextField(20);
        JTextField phoneField = new JTextField(20);
        JTextField addressField = new JTextField(20);

        nameField.setText(supplier.getName());
        phoneField.setText(supplier.getPhone() == null ? "" : supplier.getPhone());
        addressField.setText(supplier.getAddress() == null ? "" : supplier.getAddress());

        gbc.gridx = 0;
        gbc.gridy = 0;
        panel.add(new JLabel("供应商名称:"), gbc);
        gbc.gridx = 1;
        panel.add(nameField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        panel.add(new JLabel("电话:"), gbc);
        gbc.gridx = 1;
        panel.add(phoneField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        panel.add(new JLabel("地址:"), gbc);
        gbc.gridx = 1;
        panel.add(addressField, gbc);

        int result = JOptionPane.showConfirmDialog(this, panel, "编辑供应商", JOptionPane.OK_CANCEL_OPTION);
        if (result == JOptionPane.OK_OPTION) {
            String name = nameField.getText().trim();
            if (name.isEmpty()) {
                JOptionPane.showMessageDialog(this, "供应商名称不能为空！");
                return;
            }

            supplier.setName(name);
            supplier.setPhone(phoneField.getText().trim());
            supplier.setAddress(addressField.getText().trim());
            supplierDAO.update(supplier);
            loadData();
            JOptionPane.showMessageDialog(this, "编辑成功！");
        }
    }

    private void deleteSupplier() {
        int row = supplierTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要删除的供应商！");
            return;
        }

        int result = JOptionPane.showConfirmDialog(this, "确定删除该供应商吗？", "确认删除", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            int id = (int) tableModel.getValueAt(row, 0);
            supplierDAO.delete(id);
            loadData();
            JOptionPane.showMessageDialog(this, "删除成功！");
        }
    }
}
