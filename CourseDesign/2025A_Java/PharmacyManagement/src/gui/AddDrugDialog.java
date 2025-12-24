package gui;

import dao.DrugDAO;
import dao.CategoryDAO;
import dao.SupplierDAO;
import model.Drug;
import model.Category;
import model.Supplier;

import javax.swing.*;
import java.awt.*;
import java.math.BigDecimal;
import java.sql.Date;
import java.util.List;

public abstract class AddDrugDialog extends JDialog {
    private final DrugDAO drugDAO;
    private final CategoryDAO categoryDAO;
    private final SupplierDAO supplierDAO;

    private JTextField nameField;
    private JComboBox<Category> categoryBox;
    private JComboBox<Supplier> supplierBox;
    private JTextField barcodeField;
    private JTextField priceField;
    private JTextField stockField;
    private JTextArea descriptionField;
    private JTextField expireDateField;

    public AddDrugDialog(JFrame parent, DrugDAO drugDAO, CategoryDAO categoryDAO, 
                        SupplierDAO supplierDAO) {
        super(parent, "添加药品", true);
        this.drugDAO = drugDAO;
        this.categoryDAO = categoryDAO;
        this.supplierDAO = supplierDAO;

        setSize(500, 550);
        setResizable(false);
        setLocationRelativeTo(parent);
        initComponents();
    }

    private void initComponents() {
        JPanel scrollContent = new JPanel(new GridBagLayout());
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.insets = new Insets(8, 8, 8, 8);
        gbc.fill = GridBagConstraints.HORIZONTAL;

        gbc.gridx = 0;
        gbc.gridy = 0;
        scrollContent.add(new JLabel("药品名称:"), gbc);
        nameField = new JTextField(20);
        nameField.setToolTipText("必填");
        gbc.gridx = 1;
        scrollContent.add(nameField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        scrollContent.add(new JLabel("条形码:"), gbc);
        barcodeField = new JTextField(20);
        barcodeField.setToolTipText("可选");
        gbc.gridx = 1;
        scrollContent.add(barcodeField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        scrollContent.add(new JLabel("分类:"), gbc);
        categoryBox = new JComboBox<>();
        loadCategories();
        categoryBox.setToolTipText("必选");
        gbc.gridx = 1;
        scrollContent.add(categoryBox, gbc);

        gbc.gridx = 0;
        gbc.gridy = 3;
        scrollContent.add(new JLabel("供应商:"), gbc);
        supplierBox = new JComboBox<>();
        loadSuppliers();
        supplierBox.setToolTipText("必选");
        gbc.gridx = 1;
        scrollContent.add(supplierBox, gbc);

        gbc.gridx = 0;
        gbc.gridy = 4;
        scrollContent.add(new JLabel("价格:"), gbc);
        priceField = new JTextField(20);
        gbc.gridx = 1;
        scrollContent.add(priceField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 5;
        scrollContent.add(new JLabel("库存:"), gbc);
        stockField = new JTextField(20);
        gbc.gridx = 1;
        scrollContent.add(stockField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 6;
        gbc.anchor = GridBagConstraints.NORTH;
        scrollContent.add(new JLabel("描述:"), gbc);
        descriptionField = new JTextArea(4, 20);
        descriptionField.setLineWrap(true);
        descriptionField.setWrapStyleWord(true);
        JScrollPane descScrollPane = new JScrollPane(descriptionField);
        gbc.gridx = 1;
        gbc.gridwidth = 1;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        scrollContent.add(descScrollPane, gbc);
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.weightx = 0;
        gbc.weighty = 0;

        gbc.gridx = 0;
        gbc.gridy = 7;
        gbc.anchor = GridBagConstraints.WEST;
        scrollContent.add(new JLabel("过期日期:"), gbc);
        expireDateField = new JTextField(20);
        gbc.gridx = 1;
        scrollContent.add(expireDateField, gbc);

        JScrollPane mainScrollPane = new JScrollPane(scrollContent);
        
        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        mainPanel.add(mainScrollPane, BorderLayout.CENTER);

        JPanel btnPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 0));
        JButton saveBtn = new JButton("保存");
        JButton cancelBtn = new JButton("取消");

        saveBtn.addActionListener(e -> saveDrug());
        cancelBtn.addActionListener(e -> this.dispose());

        btnPanel.add(saveBtn);
        btnPanel.add(cancelBtn);

        mainPanel.add(btnPanel, BorderLayout.SOUTH);
        setContentPane(mainPanel);
    }

    private void loadCategories() {
        List<Category> categories = categoryDAO.findAll();
        for (Category c : categories) {
            categoryBox.addItem(c);
        }
    }

    private void loadSuppliers() {
        List<Supplier> suppliers = supplierDAO.findAll();
        for (Supplier s : suppliers) {
            supplierBox.addItem(s);
        }
    }

    private void saveDrug() {
        try {
            String name = nameField.getText().trim();
            if (name.isEmpty()) {
                JOptionPane.showMessageDialog(this, "药品名称不能为空！");
                return;
            }

            Category category = (Category) categoryBox.getSelectedItem();
            Supplier supplier = (Supplier) supplierBox.getSelectedItem();
            
            if (category == null || supplier == null) {
                JOptionPane.showMessageDialog(this, "请选择分类和供应商！");
                return;
            }

            BigDecimal price = new BigDecimal(priceField.getText().trim());
            int stock = Integer.parseInt(stockField.getText().trim());
            String description = descriptionField.getText().trim();
            String barcode = barcodeField.getText().trim();
            
            Drug drug = new Drug(name, category.getId(), supplier.getId(), price, stock, description, null);
            if (!barcode.isEmpty()) {
                drug.setBarcode(barcode);
            }
            
            if (!expireDateField.getText().trim().isEmpty()) {
                drug.setExpireDate(Date.valueOf(expireDateField.getText().trim()));
            }

            drugDAO.insert(drug);
            JOptionPane.showMessageDialog(this, "药品添加成功！");
            onSuccess();
            this.dispose();
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "价格和库存必须是数字！");
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "保存失败：" + ex.getMessage());
        }
    }

    protected abstract void onSuccess();
}
