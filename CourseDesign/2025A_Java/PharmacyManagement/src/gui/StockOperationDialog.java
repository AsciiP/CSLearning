package gui;

import dao.DrugDAO;
import dao.StockHistoryDAO;
import model.Drug;
import model.StockHistory;

import javax.swing.*;
import java.awt.*;

public abstract class StockOperationDialog extends JDialog {
    private final DrugDAO drugDAO;
    private final StockHistoryDAO stockHistoryDAO;
    private final Drug drug;
    private final boolean isStockIn;

    private JTextField quantityField;
    private JTextArea infoField;

    public StockOperationDialog(JFrame parent, Drug drug, boolean isStockIn, 
                               DrugDAO drugDAO, StockHistoryDAO stockHistoryDAO) {
        super(parent, isStockIn ? "进货" : "出货", true);
        this.drug = drug;
        this.isStockIn = isStockIn;
        this.drugDAO = drugDAO;
        this.stockHistoryDAO = stockHistoryDAO;

        setSize(500, 400);
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
        gbc.gridx = 1;
        scrollContent.add(new JLabel(drug.getName()), gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        scrollContent.add(new JLabel("当前库存:"), gbc);
        gbc.gridx = 1;
        scrollContent.add(new JLabel(String.valueOf(drug.getStock())), gbc);

        gbc.gridx = 0;
        gbc.gridy = 2;
        scrollContent.add(new JLabel("数量:"), gbc);
        quantityField = new JTextField(20);
        gbc.gridx = 1;
        scrollContent.add(quantityField, gbc);

        gbc.gridx = 0;
        gbc.gridy = 3;
        gbc.anchor = GridBagConstraints.NORTH;
        scrollContent.add(new JLabel("备注:"), gbc);
        infoField = new JTextArea(4, 20);
        infoField.setLineWrap(true);
        infoField.setWrapStyleWord(true);
        JScrollPane infoScrollPane = new JScrollPane(infoField);
        gbc.gridx = 1;
        gbc.fill = GridBagConstraints.BOTH;
        gbc.weightx = 1.0;
        gbc.weighty = 1.0;
        scrollContent.add(infoScrollPane, gbc);

        JScrollPane mainScrollPane = new JScrollPane(scrollContent);
        
        JPanel mainPanel = new JPanel(new BorderLayout(10, 10));
        mainPanel.setBorder(BorderFactory.createEmptyBorder(12, 12, 12, 12));
        mainPanel.add(mainScrollPane, BorderLayout.CENTER);

        JPanel btnPanel = new JPanel(new FlowLayout(FlowLayout.CENTER, 10, 0));
        JButton confirmBtn = new JButton(isStockIn ? "确认进货" : "确认出货");
        JButton cancelBtn = new JButton("取消");

        confirmBtn.addActionListener(e -> performOperation());
        cancelBtn.addActionListener(e -> this.dispose());

        btnPanel.add(confirmBtn);
        btnPanel.add(cancelBtn);

        mainPanel.add(btnPanel, BorderLayout.SOUTH);
        setContentPane(mainPanel);
    }

    private void performOperation() {
        try {
            int quantity = Integer.parseInt(quantityField.getText().trim());
            if (quantity <= 0) {
                JOptionPane.showMessageDialog(this, "数量必须大于0！");
                return;
            }

            if (!isStockIn && quantity > drug.getStock()) {
                JOptionPane.showMessageDialog(this, "出货数量不能超过库存！");
                return;
            }

            int changeAmount = isStockIn ? quantity : -quantity;
            String info = infoField.getText().trim();
            if (info.isEmpty()) {
                info = isStockIn ? "进货" : "出货";
            }

            StockHistory history = new StockHistory(drug.getId(), changeAmount, info);
            stockHistoryDAO.insert(history);

            drug.setStock(drug.getStock() + changeAmount);
            drugDAO.update(drug);

            JOptionPane.showMessageDialog(this, (isStockIn ? "进货" : "出货") + "成功！");
            onSuccess();
            this.dispose();
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "数量必须是数字！");
        } catch (Exception ex) {
            JOptionPane.showMessageDialog(this, "操作失败：" + ex.getMessage());
        }
    }

    protected abstract void onSuccess();
}
