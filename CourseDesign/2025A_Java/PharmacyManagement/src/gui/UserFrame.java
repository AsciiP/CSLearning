package gui;

import dao.DrugDAO;
import dao.SalesDAO;
import dao.SupplierDAO;
import model.Drug;
import model.Sales;

import javax.swing.*;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.math.BigDecimal;
import java.util.List;

public class UserFrame extends JFrame {
    
    private final DrugDAO drugDAO = new DrugDAO();
    private final SalesDAO salesDAO = new SalesDAO();
    private final SupplierDAO supplierDAO = new SupplierDAO();
    private int userId;
    
    private JTable drugTable;
    private DefaultTableModel drugTableModel;
    private JTable cartTable;
    private DefaultTableModel cartTableModel;
    private JLabel totalLabel;
    private JTextField quantityField;
    private JTextField barcodeField;

    public UserFrame(String username, int userId) {
        this.userId = userId;
        setTitle("药店管理系统 - 收银员 - " + username);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(1200, 700);
        setLocationRelativeTo(null);
        setResizable(true);

        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

        // 左侧：药品列表
        JPanel leftPanel = createLeftPanel();
        mainPanel.add(leftPanel, BorderLayout.WEST);

        // 右侧：购物车
        JPanel rightPanel = createRightPanel();
        mainPanel.add(rightPanel, BorderLayout.CENTER);

        setContentPane(mainPanel);
        setVisible(true);
        
        loadDrugData();
    }

    private JPanel createLeftPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createTitledBorder("可购药品"));
        panel.setPreferredSize(new Dimension(600, 0));

        // 顶部：条形码扫描框
        JPanel barcodePanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JLabel barcodeLabel = new JLabel("条形码:");
        barcodeField = new JTextField(20);
        barcodePanel.add(barcodeLabel);
        barcodePanel.add(barcodeField);
        
        // 条形码输入框的KeyListener - 检测回车键
        barcodeField.addKeyListener(new KeyAdapter() {
            @Override
            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_ENTER) {
                    String barcode = barcodeField.getText().trim();
                    if (barcode.isEmpty()) {
                        // 条形码为空时，按回车执行结账操作（需确认）
                        checkoutWithConfirmation();
                    } else {
                        scanBarcode();
                        barcodeField.setText("");
                    }
                }
            }
        });
        
        // 条形码输入框的DocumentListener - 检测13位数字自动扫描
        barcodeField.getDocument().addDocumentListener(new DocumentListener() {
            @Override
            public void insertUpdate(DocumentEvent e) {
                checkBarcodeLength();
            }
            
            @Override
            public void removeUpdate(DocumentEvent e) {
                // 不需要处理删除事件
            }
            
            @Override
            public void changedUpdate(DocumentEvent e) {
                // 不需要处理属性变化事件
            }
            
            private void checkBarcodeLength() {
                SwingUtilities.invokeLater(() -> {
                    String text = barcodeField.getText();
                    if (text.length() == 13 && text.matches("\\d{13}")) {
                        scanBarcode();
                        barcodeField.setText("");
                    }
                });
            }
        });
        
        panel.add(barcodePanel, BorderLayout.NORTH);

        String[] columns = {"ID", "药品名称", "供应商", "条形码", "单价", "库存"};
        drugTableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        drugTable = new JTable(drugTableModel);
        drugTable.getColumnModel().getColumn(0).setPreferredWidth(50); // ID
        drugTable.getColumnModel().getColumn(1).setPreferredWidth(200); // 药品名称
        drugTable.getColumnModel().getColumn(2).setPreferredWidth(150); // 供应商
        drugTable.getColumnModel().getColumn(3).setPreferredWidth(150); // 条形码
        
        drugTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        drugTable.setRowHeight(25);
        
        // 添加双击事件监听器
        drugTable.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    addToCart();
                }
            }
        });
        
        JScrollPane scrollPane = new JScrollPane(drugTable);
        panel.add(scrollPane, BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JLabel qtyLabel = new JLabel("数量:");
        quantityField = new JTextField(8);
        quantityField.setText("1");
        JButton addToCartBtn = new JButton("加入购物车");
        JButton logoutBtn = new JButton("退出登录");

        addToCartBtn.addActionListener(e -> addToCart());
        logoutBtn.addActionListener(e -> logout());

        bottomPanel.add(qtyLabel);
        bottomPanel.add(quantityField);
        bottomPanel.add(addToCartBtn);
        bottomPanel.add(new JSeparator(JSeparator.VERTICAL));
        bottomPanel.add(logoutBtn);

        panel.add(bottomPanel, BorderLayout.SOUTH);
        
        // 加载后将光标放在条形码输入框
        SwingUtilities.invokeLater(() -> barcodeField.requestFocus());
        
        return panel;
    }

    private JPanel createRightPanel() {
        JPanel panel = new JPanel(new BorderLayout(10, 10));
        panel.setBorder(BorderFactory.createTitledBorder("购物车"));

        String[] columns = {"药品ID", "药品名称", "单价", "数量", "小计"};
        cartTableModel = new DefaultTableModel(columns, 0) {
            @Override
            public boolean isCellEditable(int row, int column) {
                return false;
            }
        };
        
        cartTable = new JTable(cartTableModel);
        cartTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        cartTable.setRowHeight(25);
        
        JScrollPane scrollPane = new JScrollPane(cartTable);
        panel.add(scrollPane, BorderLayout.CENTER);

        JPanel bottomPanel = new JPanel();
        bottomPanel.setLayout(new BoxLayout(bottomPanel, BoxLayout.Y_AXIS));
        
        // 第一行：操作按钮
        JPanel firstRow = new JPanel(new FlowLayout(FlowLayout.LEFT, 10, 5));
        JButton increaseBtn = new JButton("数量+1");
        JButton decreaseBtn = new JButton("数量-1");
        JButton removeBtn = new JButton("删除商品");
        JButton clearBtn = new JButton("清空购物车");
        increaseBtn.addActionListener(e -> increaseQuantity());
        decreaseBtn.addActionListener(e -> decreaseQuantity());
        removeBtn.addActionListener(e -> removeFromCart());
        clearBtn.addActionListener(e -> clearCart());
        firstRow.add(increaseBtn);
        firstRow.add(decreaseBtn);
        firstRow.add(removeBtn);
        firstRow.add(clearBtn);
        
        // 第二行：合计和结账
        JPanel secondRow = new JPanel(new FlowLayout(FlowLayout.RIGHT, 10, 5));
        totalLabel = new JLabel("合计: ¥0.00");
        totalLabel.setFont(new Font("Microsoft YaHei", Font.BOLD, 16));
        totalLabel.setForeground(Color.RED);
        JButton checkoutBtn = new JButton("确认结账");
        checkoutBtn.setFont(new Font("Microsoft YaHei", Font.BOLD, 14));
        checkoutBtn.setBackground(new Color(76, 175, 80));
        checkoutBtn.setForeground(Color.WHITE);
        checkoutBtn.setPreferredSize(new Dimension(100, 35));
        checkoutBtn.addActionListener(e -> checkout());
        secondRow.add(totalLabel);
        secondRow.add(checkoutBtn);
        
        bottomPanel.add(firstRow);
        bottomPanel.add(secondRow);

        panel.add(bottomPanel, BorderLayout.SOUTH);
        return panel;
    }

    private void loadDrugData() {
        drugTableModel.setRowCount(0);
        List<Drug> drugs = drugDAO.findAll();
        for (Drug drug : drugs) {
            String supplierName = "未指定";
            if (drug.getSupplierId() > 0) {
                var supplier = supplierDAO.findById(drug.getSupplierId());
                if (supplier != null) {
                    supplierName = supplier.getName();
                }
            }
            Object[] row = {
                drug.getId(),
                drug.getName(),
                supplierName,
                drug.getBarcode() == null ? "" : drug.getBarcode(),
                drug.getPrice(),
                drug.getStock()
            };
            drugTableModel.addRow(row);
        }
    }

    private void addToCart() {
        int row = drugTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请先选择药品！");
            return;
        }

        try {
            int quantity = Integer.parseInt(quantityField.getText().trim());
            if (quantity <= 0) {
                JOptionPane.showMessageDialog(this, "数量必须大于0！");
                return;
            }

            int drugId = (int) drugTableModel.getValueAt(row, 0);
            String drugName = (String) drugTableModel.getValueAt(row, 1);
            BigDecimal price = new BigDecimal(drugTableModel.getValueAt(row, 4).toString());
            int stock = (int) drugTableModel.getValueAt(row, 5);

            if (quantity > stock) {
                JOptionPane.showMessageDialog(this, "库存不足！当前库存: " + stock);
                return;
            }

            // 检查是否已经在购物车中
            for (int i = 0; i < cartTableModel.getRowCount(); i++) {
                if ((int) cartTableModel.getValueAt(i, 0) == drugId) {
                    // 更新数量
                    int oldQty = (int) cartTableModel.getValueAt(i, 3);
                    int newQty = oldQty + quantity;
                    if (newQty > stock) {
                        JOptionPane.showMessageDialog(this, "库存不足！当前库存: " + stock);
                        return;
                    }
                    BigDecimal subtotal = price.multiply(new BigDecimal(newQty));
                    cartTableModel.setValueAt(newQty, i, 3);
                    cartTableModel.setValueAt(subtotal, i, 4);
                    updateTotal();
                    return;
                }
            }

            // 新增到购物车
            BigDecimal subtotal = price.multiply(new BigDecimal(quantity));
            Object[] cartRow = {
                drugId,
                drugName,
                price,
                quantity,
                subtotal
            };
            cartTableModel.addRow(cartRow);
            updateTotal();
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "数量必须是整数！");
        }
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void scanBarcode() {
        String barcode = barcodeField.getText().trim();
        if (barcode.isEmpty()) {
            return;
        }

        Drug drug = drugDAO.findByBarcode(barcode);
        if (drug == null) {
            JOptionPane.showMessageDialog(this, "未找到条形码: " + barcode);
            return;
        }

        if (drug.getStock() <= 0) {
            JOptionPane.showMessageDialog(this, "该药品库存不足！");
            return;
        }

        // 数量默认为1
        int quantity = 1;
        BigDecimal price = drug.getPrice();

        // 检查是否已经在购物车中
        for (int i = 0; i < cartTableModel.getRowCount(); i++) {
            if ((int) cartTableModel.getValueAt(i, 0) == drug.getId()) {
                // 更新数量
                int oldQty = (int) cartTableModel.getValueAt(i, 3);
                int newQty = oldQty + quantity;
                if (newQty > drug.getStock()) {
                    JOptionPane.showMessageDialog(this, "库存不足！当前库存: " + drug.getStock());
                    return;
                }
                BigDecimal subtotal = price.multiply(new BigDecimal(newQty));
                cartTableModel.setValueAt(newQty, i, 3);
                cartTableModel.setValueAt(subtotal, i, 4);
                updateTotal();
                return;
            }
        }

        // 新增到购物车
        BigDecimal subtotal = price.multiply(new BigDecimal(quantity));
        Object[] cartRow = {
            drug.getId(),
            drug.getName(),
            price,
            quantity,
            subtotal
        };
        cartTableModel.addRow(cartRow);
        updateTotal();
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void removeFromCart() {
        int row = cartTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要删除的商品！");
            return;
        }
        cartTableModel.removeRow(row);
        updateTotal();
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void increaseQuantity() {
        int row = cartTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要修改的商品！");
            return;
        }

        int drugId = (int) cartTableModel.getValueAt(row, 0);
        int currentQty = (int) cartTableModel.getValueAt(row, 3);
        BigDecimal price = new BigDecimal(cartTableModel.getValueAt(row, 2).toString());

        // 获取库存信息
        Drug drug = drugDAO.findById(drugId);
        if (drug == null) {
            JOptionPane.showMessageDialog(this, "药品信息未找到！");
            return;
        }

        int newQty = currentQty + 1;
        if (newQty > drug.getStock()) {
            JOptionPane.showMessageDialog(this, "库存不足！当前库存: " + drug.getStock());
            return;
        }

        BigDecimal subtotal = price.multiply(new BigDecimal(newQty));
        cartTableModel.setValueAt(newQty, row, 3);
        cartTableModel.setValueAt(subtotal, row, 4);
        updateTotal();
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void decreaseQuantity() {
        int row = cartTable.getSelectedRow();
        if (row < 0) {
            JOptionPane.showMessageDialog(this, "请选择要修改的商品！");
            return;
        }

        int currentQty = (int) cartTableModel.getValueAt(row, 3);
        
        if (currentQty <= 1) {
            // 数量为1时，减少就是删除
            int result = JOptionPane.showConfirmDialog(this, 
                "数量为1，是否要删除该商品？", "确认删除", JOptionPane.YES_NO_OPTION);
            if (result == JOptionPane.YES_OPTION) {
                cartTableModel.removeRow(row);
                updateTotal();
            }
            return;
        }

        BigDecimal price = new BigDecimal(cartTableModel.getValueAt(row, 2).toString());
        int newQty = currentQty - 1;
        BigDecimal subtotal = price.multiply(new BigDecimal(newQty));
        
        cartTableModel.setValueAt(newQty, row, 3);
        cartTableModel.setValueAt(subtotal, row, 4);
        updateTotal();
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void clearCart() {
        int result = JOptionPane.showConfirmDialog(this, "确定要清空购物车吗？", "确认清空", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            cartTableModel.setRowCount(0);
            updateTotal();
        }
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void updateTotal() {
        BigDecimal total = BigDecimal.ZERO;
        for (int i = 0; i < cartTableModel.getRowCount(); i++) {
            BigDecimal subtotal = new BigDecimal(cartTableModel.getValueAt(i, 4).toString());
            total = total.add(subtotal);
        }
        totalLabel.setText(String.format("合计: ¥%.2f", total));
    }

    private void checkout() {
        if (cartTableModel.getRowCount() == 0) {
            JOptionPane.showMessageDialog(this, "购物车为空，无法结账！");
            return;
        }

        try {
            BigDecimal totalAmount = new BigDecimal(0);
            
            for (int i = 0; i < cartTableModel.getRowCount(); i++) {
                int drugId = (int) cartTableModel.getValueAt(i, 0);
                int quantity = (int) cartTableModel.getValueAt(i, 3);
                BigDecimal subtotal = new BigDecimal(cartTableModel.getValueAt(i, 4).toString());
                
                // 保存销售记录
                Sales sale = new Sales(drugId, quantity, subtotal, userId);
                salesDAO.insert(sale);
                
                // 更新药品库存
                Drug drug = drugDAO.findById(drugId);
                drug.setStock(drug.getStock() - quantity);
                drugDAO.update(drug);
                
                totalAmount = totalAmount.add(subtotal);
            }

            // 显示结账成功信息
            JOptionPane.showMessageDialog(this, 
                String.format("结账成功！\n总金额: ¥%.2f\n已保存销售记录", totalAmount));
            
            // 清空购物车，刷新药品列表
            cartTableModel.setRowCount(0);
            updateTotal();
            loadDrugData();
            quantityField.setText("1");
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "结账失败：" + ex.getMessage());
        }
        
        // 操作完成后，光标回到条形码输入框
        barcodeField.requestFocus();
    }

    private void checkoutWithConfirmation() {
        if (cartTableModel.getRowCount() == 0) {
            JOptionPane.showMessageDialog(this, "购物车为空，无法结账！");
            return;
        }
        
        // 计算总金额用于显示
        BigDecimal totalAmount = BigDecimal.ZERO;
        for (int i = 0; i < cartTableModel.getRowCount(); i++) {
            BigDecimal subtotal = new BigDecimal(cartTableModel.getValueAt(i, 4).toString());
            totalAmount = totalAmount.add(subtotal);
        }
        
        // 二次确认
        int result = JOptionPane.showConfirmDialog(this, 
            String.format("确认结账吗？\n总金额: ¥%.2f", totalAmount), 
            "确认结账", 
            JOptionPane.YES_NO_OPTION);
        
        if (result == JOptionPane.YES_OPTION) {
            checkout();
        } else {
            // 取消结账，光标回到条形码输入框
            barcodeField.requestFocus();
        }
    }

    private void logout() {
        int result = JOptionPane.showConfirmDialog(this, "确定要退出登录吗？", "确认退出", JOptionPane.YES_NO_OPTION);
        if (result == JOptionPane.YES_OPTION) {
            this.dispose();
            new LoginFrame();
        }
    }
}
